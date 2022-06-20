#include "psarc.h"

#include <QDir>

#include "3rdparty/Rijndael/Rijndael.h"
#include "common.h"

static const unsigned char PsarcKey[32] = {0xC5, 0x3D, 0xB2, 0x38, 0x70, 0xA1, 0xA2, 0xF7, 0x1C, 0xAE, 0x64,
                                           0x06, 0x1F, 0xDD, 0x0E, 0x11, 0x57, 0x30, 0x9D, 0xC8, 0x52, 0x04,
                                           0xD4, 0xC5, 0xBF, 0xDF, 0x25, 0x09, 0x0D, 0xF2, 0x57, 0x2C};

bool RS::PSARC::inflateEntry(uint32_t&          entry,
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
        if (m_entries[entry].m_length != 0)
        {
            file.seek(m_entries[entry].m_zOffset);
            uint32_t zIndex = m_entries[entry].m_zIndex;
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
                                   qMin(m_entries[entry].m_length - (zIndex - m_entries[entry].m_zIndex) * cBlockSize,
                                        (uint64_t)cBlockSize),
                                   compressedData);
                        stream.write(uncompressedData);
                    }
                    else
                        stream.write(compressedData, zBlocks[zIndex]);
                }
                zIndex++;
            } while (stream.pos() < m_entries[entry].m_length);
        }

        if (entry == 0)
        {
            stream.close();

            QFile reader(fileName);
            reader.open(QIODevice::ReadOnly);

            m_entries[0].m_name = (char*)"NamesBlock.bin";
            for (uint32_t i = 1; i < m_entries.size(); i++)
            {
                m_entries[i].m_name = reader.readLine();
                m_entries[i].m_name.remove(m_entries[i].m_name.length() - 1, 1);
                m_entries[i].m_name.replace("/", "\\");
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

void RS::PSARC::setPsarcFile(const QString& newPsarcFileName)
{
    m_psarcFile.setFileName(newPsarcFileName);
}

RS::PSARC::PSARC(const QString& archiveName) : m_psarcFile(archiveName) {}

bool RS::PSARC::unarchive()
{
    if (m_psarcFile.open(QIODevice::ReadOnly))
    {
        if (READ_BE_UINT32((uint8_t*)m_psarcFile.read(4).constData()) == kPSARCMagicNumber)
        {
            m_psarcFile.seek(8);
            if (READ_BE_UINT32((uint8_t*)m_psarcFile.read(4).constData()) == 0x7a6c6962)
            {
                uint32_t zSize = READ_BE_UINT32((uint8_t*)m_psarcFile.read(4).constData());
                m_psarcFile.seek(headerSize);
                uint32_t _numEntries = READ_BE_UINT32((uint8_t*)m_psarcFile.read(4).constData());
                m_psarcFile.seek(24);
                uint32_t cBlockSize = READ_BE_UINT32((uint8_t*)m_psarcFile.read(4).constData());

                uint8_t  zType = 1;
                uint32_t i     = 256;
                do
                {
                    i *= 256;
                    zType = (uint8_t)(zType + 1);
                } while (i < cBlockSize);

                m_psarcFile.seek(32);

                QByteArray decryptedToc;
                decryptedToc.resize(zSize);
                CRijndael rijndael;

                rijndael.MakeKey(PsarcKey, (unsigned char*)CRijndael::sm_chain0, 32, 16);
                rijndael.Decrypt((unsigned char*)m_psarcFile.read(zSize - 32).constData(),
                                 (unsigned char*)decryptedToc.data(),
                                 zSize & ~31,
                                 CRijndael::CFB);

                qsizetype decryptedTocOffset = 0;
                m_entries.resize(_numEntries);
                for (uint32_t i = 0; i < m_entries.size(); i++)
                {
                    decryptedTocOffset += 16;

                    m_entries[i].m_id = i;

                    m_entries[i].m_zIndex = READ_BE_UINT32((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                    decryptedTocOffset += 4;

                    m_entries[i].m_length = READ_BE_INT40((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                    decryptedTocOffset += 5;

                    m_entries[i].m_zOffset = READ_BE_INT40((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
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

                QString unpackDir = QFileInfo(m_psarcFile).completeBaseName();
                for (uint32_t i = 0; i < m_entries.size(); i++)
                {
                    inflateEntry(i, zBlocks, cBlockSize, unpackDir + "\\" + m_entries[i].m_name, m_psarcFile);
                }

                return true;
            }
        }

        m_psarcFile.close();
    }
    return false;
}
