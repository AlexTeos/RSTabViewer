#include "sng.h"

#include <QFileInfo>

#include "Rijndael.h"
#include "common.h"
#include "psarc.h"

static const unsigned char SngKeyPC[32] = {0xCB, 0x64, 0x8D, 0xF3, 0xD1, 0x2A, 0x16, 0xBF, 0x71, 0x70, 0x14,
                                           0x14, 0xE6, 0x96, 0x19, 0xEC, 0x17, 0x1C, 0xCA, 0x5D, 0x2A, 0x14,
                                           0x2E, 0x3E, 0x59, 0xDE, 0x7A, 0xDD, 0xA1, 0x8A, 0x3A, 0x30};

SNG::SNG(const QString& sngFileName) : m_sngFile(sngFileName) {}

bool SNG::decrypt()
{
    if (m_sngFile.open(QIODevice::ReadOnly))
    {
        if (READ_LE_UINT32((uint8_t*)m_sngFile.read(4).constData()) == 0x4a)
        {
            if (READ_LE_UINT32((uint8_t*)m_sngFile.read(4).constData()) == 0x03)
            {
                qsizetype  offset      = 8;
                uint64_t   writeOffset = 0;
                uint8_t    blockLength = 16;
                QByteArray decryptedSng;
                decryptedSng.resize(m_sngFile.size() + 32);
                //entry.setDecryptedLength(entry.getLength());
                //entry.setDecryptedData(decryptedSng);
                char iv[16];
                for (int i = 0; i < 16; i++)
                {
                    m_sngFile.read(&iv[i], 1);
                }
                CRijndael rijndael;

                do
                {
                    rijndael.MakeKey(SngKeyPC, (unsigned char*)iv, 32, blockLength);
                    rijndael.Decrypt((unsigned char*)m_sngFile.read(blockLength).constData(),
                                     (unsigned char*)decryptedSng.data() + writeOffset,
                                     blockLength,
                                     CRijndael::CFB);
                    offset += blockLength;
                    writeOffset += blockLength;
                    bool carry = true;
                    for (int j = 15; j >= 0 && carry; j--)
                    {
                        iv[j]++;
                        carry = (iv[j] == 0);
                    }
                } while (offset < m_sngFile.size());
                m_sngFile.close();

                QByteArray uncompressedData;
                uLongf     uncompressedSize = READ_LE_UINT32((uint8_t*)decryptedSng.constData());
                decryptedSng.erase(decryptedSng.begin(), decryptedSng.begin() + 4);
                uncompress(uncompressedData, uncompressedSize, decryptedSng);

                QFile resultFile(m_sngFile.fileName().insert(
                    m_sngFile.fileName().length() - QFileInfo(m_sngFile).suffix().length() - 1, "_uncompressed"));

                if (resultFile.open(QIODevice::WriteOnly))
                {
                    resultFile.write(uncompressedData);
                    resultFile.close();
                    return true;
                }
            }
        }
    }
    return false;
}

void SNG::setSngFile(const QString& newSngFileName)
{
    m_sngFile.setFileName(newSngFileName);
}
