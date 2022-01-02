#include "psarc.h"

#include <QDir>

#include "Rijndael.h"

static const unsigned char PsarcKey[32] = {0xC5, 0x3D, 0xB2, 0x38, 0x70, 0xA1, 0xA2, 0xF7, 0x1C, 0xAE, 0x64,
                                           0x06, 0x1F, 0xDD, 0x0E, 0x11, 0x57, 0x30, 0x9D, 0xC8, 0x52, 0x04,
                                           0xD4, 0xC5, 0xBF, 0xDF, 0x25, 0x09, 0x0D, 0xF2, 0x57, 0x2C};

bool PSARC::uncompress(QByteArray& dest, uLongf destLen, QByteArray& source)
{
    uLongf     destLenFlip = READ_BE_UINT32((uint8_t*)&destLen);
    QByteArray arr;
    arr.append((char*)&destLenFlip, 4);
    arr.append(source);
    dest = qUncompress(arr);
    return true;
}

bool PSARC::inflateEntry(uint32_t&          entry,
                         QVector<uint32_t>& zBlocks,
                         uint32_t&          cBlockSize,
                         QString            fileName,
                         QFile&             file)
{
    if (entry == 0) fileName = "psarc.temp";

    QDir().mkpath(QFileInfo(fileName).path());

    QFile stream(fileName);
    if (stream.open(QIODevice::WriteOnly))
    {
        if (_entries[entry]._length != 0)
        {
            file.seek(_entries[entry]._zOffset);
            uint32_t zIndex = _entries[entry]._zIndex;
            do
            {
                if (zBlocks[zIndex] == 0)
                {
                    stream.write(file.read(cBlockSize), cBlockSize);
                }
                else
                {
                    uint16_t   isGzipped      = READ_BE_UINT16((uint8_t*)file.peek(2).constData());
                    QByteArray compressedData = file.read(zBlocks[zIndex]);
                    if (isGzipped == 0x78da)
                    {
                        QByteArray uncompressData;
                        uncompress(
                            uncompressData,
                            qMin(_entries[entry]._length - (zIndex - _entries[entry]._zIndex) * cBlockSize, cBlockSize),
                            compressedData);
                        stream.write(uncompressData);
                    }
                    else
                        stream.write(compressedData, zBlocks[zIndex]);
                }
                zIndex++;
            } while (stream.pos() < _entries[entry]._length);
        }

        if (entry == 0)
        {
            stream.close();

            QFile reader(fileName);
            reader.open(QIODevice::ReadOnly);

            _entries[0]._name = (char*)"NamesBlock.bin";
            for (uint32_t i = 1; i < _entries.size(); i++)
            {
                _entries[i]._name = reader.readLine();
                _entries[i]._name.remove(_entries[i]._name.length() - 1, 1);
                _entries[i]._name.replace("/", "\\");
            }

            reader.close();
            stream.remove();
        }
        else
        {
            stream.close();
            return false;
        }
        return true;
    }
    return false;
}

bool PSARC::unarchive(const QString& archiveName)
{
    QFile _f(archiveName);
    if (_f.open(QIODevice::ReadOnly))
    {
        if (READ_BE_UINT32((uint8_t*)_f.read(4).constData()) == kPSARCMagicNumber)
        {
            _f.seek(8);
            if (READ_BE_UINT32((uint8_t*)_f.read(4).constData()) == 0x7a6c6962)
            {
                uint32_t zSize = READ_BE_UINT32((uint8_t*)_f.read(4).constData());
                _f.seek(headerSize);
                uint32_t _numEntries = READ_BE_UINT32((uint8_t*)_f.read(4).constData());
                _f.seek(24);
                uint32_t cBlockSize = READ_BE_UINT32((uint8_t*)_f.read(4).constData());

                uint8_t  zType = 1;
                uint32_t i     = 256;
                do
                {
                    i *= 256;
                    zType = (uint8_t)(zType + 1);
                } while (i < cBlockSize);

                _f.seek(32);

                QByteArray encryptedToc;
                encryptedToc.resize(zSize);
                CRijndael rijndael;

                rijndael.MakeKey(PsarcKey, (unsigned char*)CRijndael::sm_chain0, 32, 16);
                rijndael.Decrypt((unsigned char*)_f.read(zSize - 32).constData(),
                                 (unsigned char*)encryptedToc.data(),
                                 zSize & ~31,
                                 CRijndael::CFB);

                qsizetype encryptedTocOffset = 0;
                _entries.resize(_numEntries);
                for (uint32_t i = 0; i < _entries.size(); i++)
                {
                    encryptedTocOffset += 16;

                    _entries[i]._id = i;

                    _entries[i]._zIndex = READ_BE_UINT32((uint8_t*)encryptedToc.constData() + encryptedTocOffset);
                    encryptedTocOffset += 4;

                    _entries[i]._length = READ_BE_INT40((uint8_t*)encryptedToc.constData() + encryptedTocOffset);
                    encryptedTocOffset += 5;

                    _entries[i]._zOffset = READ_BE_INT40((uint8_t*)encryptedToc.constData() + encryptedTocOffset);
                    encryptedTocOffset += 5;
                }

                uint32_t          numBlocks = (zSize - encryptedTocOffset - headerSize) / zType;
                QVector<uint32_t> zBlocks;
                zBlocks.resize(numBlocks);
                for (uint32_t i = 0; i < numBlocks; i++)
                {
                    switch (zType)
                    {
                        case 2:
                            zBlocks[i] = READ_BE_UINT16((uint8_t*)encryptedToc.constData() + encryptedTocOffset);
                            encryptedTocOffset += 2;
                            break;

                        case 3:
                            zBlocks[i] = READ_BE_INT24((uint8_t*)encryptedToc.constData() + encryptedTocOffset);
                            encryptedTocOffset += 3;
                            break;

                        case 4:
                            zBlocks[i] = READ_BE_UINT32((uint8_t*)encryptedToc.constData() + encryptedTocOffset);
                            encryptedTocOffset += 4;
                            break;
                    }
                }

                QString unpackDir = QFileInfo(_f).completeBaseName();
                for (uint32_t i = 0; i < _entries.size(); i++)
                {
                    inflateEntry(i, zBlocks, cBlockSize, unpackDir + "\\" + _entries[i]._name, _f);
                }

                return true;
            }
        }

        _f.close();
    }
    return false;
}
