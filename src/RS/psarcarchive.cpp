#include "psarcarchive.h"

#include <QDir>

#include "3rdparty/Rijndael/Rijndael.h"
#include "common.h"

static const unsigned char PsarcKey[32] = {0xC5, 0x3D, 0xB2, 0x38, 0x70, 0xA1, 0xA2, 0xF7, 0x1C, 0xAE, 0x64,
                                           0x06, 0x1F, 0xDD, 0x0E, 0x11, 0x57, 0x30, 0x9D, 0xC8, 0x52, 0x04,
                                           0xD4, 0xC5, 0xBF, 0xDF, 0x25, 0x09, 0x0D, 0xF2, 0x57, 0x2C};

bool RS::PSARCArchive::inflateEntry(uint32_t&            entry,
                                    QVector<uint32_t>&   zBlocks,
                                    uint32_t&            cBlockSize,
                                    QString              fileName,
                                    QFile&               file,
                                    QVector<PSARCEntry>& entries)
{
    if (QDir().mkpath(QFileInfo(fileName).path()))
    {
        QFile stream(fileName);
        if (stream.open(QIODevice::WriteOnly))
        {
            file.seek(entries[entry].m_zOffset);
            uint32_t zIndex = entries[entry].m_zIndex;
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
                        QByteArray uncompressedData;
                        uncompress(uncompressedData,
                                   qMin(entries[entry].m_length - (zIndex - entries[entry].m_zIndex) * cBlockSize,
                                        (uint64_t)cBlockSize),
                                   compressedData);
                        stream.write(uncompressedData);
                    }
                    else
                        stream.write(compressedData, zBlocks[zIndex]);
                }
                zIndex++;
                if (zIndex >= zBlocks.size())
                {
                    // TODO: handle somehow
                    break;
                }
            } while (stream.pos() < entries[entry].m_length);

            if (entry == 0)
            {
                stream.close();

                QFile reader(fileName);
                reader.open(QIODevice::ReadOnly);

                entries[0].m_name = (char*)"NamesBlock.bin";
                for (uint32_t i = 1; i < entries.size(); i++)
                {
                    entries[i].m_name = reader.readLine();
                    entries[i].m_name.remove(entries[i].m_name.length() - 1, 1);
                    //entries[i].m_name.replace("/", "\\");
                }

                reader.close();
                stream.remove();
            }
            else
            {
                stream.close();
            }
            return true;
        }
    }
    return false;
}

bool RS::PSARCArchive::unarchive(const QString& archiveName, const QString& unpackDir)
{
    QVector<PSARCEntry> entries;

    QFile psarcFile(archiveName);
    if (psarcFile.open(QIODevice::ReadOnly))
    {
        if (READ_BE_UINT32((uint8_t*)psarcFile.read(4).constData()) == kPSARCMagicNumber)
        {
            psarcFile.seek(8);
            if (READ_BE_UINT32((uint8_t*)psarcFile.read(4).constData()) == 0x7a6c6962)
            {
                uint32_t zSize = READ_BE_UINT32((uint8_t*)psarcFile.read(4).constData());
                psarcFile.seek(headerSize);
                uint32_t _numEntries = READ_BE_UINT32((uint8_t*)psarcFile.read(4).constData());
                psarcFile.seek(24);
                uint32_t cBlockSize = READ_BE_UINT32((uint8_t*)psarcFile.read(4).constData());

                uint8_t  zType = 1;
                uint32_t i     = 256;
                do
                {
                    i *= 256;
                    zType = (uint8_t)(zType + 1);
                } while (i < cBlockSize);

                psarcFile.seek(32);

                QByteArray decryptedToc;
                decryptedToc.resize(zSize);
                CRijndael rijndael;

                rijndael.MakeKey(PsarcKey, (unsigned char*)CRijndael::sm_chain0, 32, 16);
                rijndael.Decrypt((unsigned char*)psarcFile.read(zSize - 32).constData(),
                                 (unsigned char*)decryptedToc.data(),
                                 zSize & ~31,
                                 CRijndael::CFB);

                qsizetype decryptedTocOffset = 0;
                entries.resize(_numEntries);
                for (uint32_t i = 0; i < entries.size(); i++)
                {
                    decryptedTocOffset += 16;

                    entries[i].m_id = i;

                    entries[i].m_zIndex = READ_BE_UINT32((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                    decryptedTocOffset += 4;

                    entries[i].m_length = READ_BE_INT40((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                    decryptedTocOffset += 5;

                    entries[i].m_zOffset = READ_BE_INT40((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                    decryptedTocOffset += 5;
                }

                uint32_t          numBlocks = (zSize - decryptedTocOffset - headerSize) / zType;
                QVector<uint32_t> zBlocks;
                zBlocks.resize(numBlocks);
                for (uint32_t i = 0; i < numBlocks; i++)
                {
                    switch (zType)
                    {
                        case 2:
                            zBlocks[i] = READ_BE_UINT16((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                            decryptedTocOffset += 2;
                            break;

                        case 3:
                            zBlocks[i] = READ_BE_INT24((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                            decryptedTocOffset += 3;
                            break;

                        case 4:
                            zBlocks[i] = READ_BE_UINT32((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                            decryptedTocOffset += 4;
                            break;
                    }
                }

                QString songName = "";
                for (uint32_t i = 0; i < entries.size(); i++)
                {
                    if (entries[i].m_length != 0)
                    {
                        if (i == 0 or entries[i].m_name.contains(QRegExp("\\.json|\\_256.dds|\\.sng|\\.wem|\\.bnk")))
                        {
                            songName = "";
                            if (i != 0 and not entries[i].m_name.contains(".wem"))
                            {
                                songName = entries[i].m_name;
                                songName.remove(0, songName.lastIndexOf("/") + 1);
                                if (entries[i].m_name.contains(".dds"))
                                    songName.remove(songName.indexOf("album_"), 6);
                                else if (entries[i].m_name.contains(".bnk"))
                                {
                                    songName.remove(songName.indexOf("song_"), 5);
                                    songName.chop(4);
                                }
                                songName.remove(QRegExp("_.*"));
                                songName += "/";
                            }
                            if (not inflateEntry(i,
                                                 zBlocks,
                                                 cBlockSize,
                                                 unpackDir + "/" + songName +
                                                     (i == 0 ? "psarc.temp" : entries[i].m_name),
                                                 psarcFile,
                                                 entries))
                                return false;
                        }
                    }
                }

                return true;
            }
        }

        psarcFile.close();
    }
    return false;
}
