#include "sng.h"

#include <QFileInfo>

#include "Rijndael.h"
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

                m_sngDecryptedFile.setFileName(m_sngFile.fileName().insert(
                    m_sngFile.fileName().length() - QFileInfo(m_sngFile).suffix().length() - 1, "_uncompressed"));

                if (m_sngDecryptedFile.open(QIODevice::WriteOnly))
                {
                    m_sngDecryptedFile.write(uncompressedData);
                    m_sngDecryptedFile.close();
                    return true;
                }
            }
        }
    }
    return false;
}

bool SNG::dummyRead(QIODevice& input, const qint64& structureSize, const qint64& additionalSize, uint32_t& count)
{
    qint64 fileSize = input.size();
    count           = READ_LE_UINT32((uint8_t*)input.read(4).constData());
    input.seek(input.pos() + additionalSize + structureSize * count);
    return input.pos() <= fileSize;
}

bool SNG::parse()
{
    if (m_sngDecryptedFile.open(QIODevice::ReadOnly))
    {
        uint32_t count;

        if (not dummyRead(m_sngDecryptedFile, 16, 0, count)) return false;   // Beat
        if (not dummyRead(m_sngDecryptedFile, 44, 0, count)) return false;   // Phrase
        if (not parseChords(m_sngDecryptedFile, m_chords)) return false;     // Chord
        if (not dummyRead(m_sngDecryptedFile, 2376, 0, count)) return false; // ChordsNote
        if (not dummyRead(m_sngDecryptedFile, 60, 0, count)) return false;   // Vocals
        if (count > 0)
        {
            uint32_t localCount;
            if (not dummyRead(m_sngDecryptedFile, 32, 0, localCount)) return false;  // Header
            if (not dummyRead(m_sngDecryptedFile, 144, 0, localCount)) return false; // Texture
            if (not dummyRead(m_sngDecryptedFile, 44, 0, localCount)) return false;  // Definition
        }
        if (not dummyRead(m_sngDecryptedFile, 24, 0, count)) return false; // PhrasesIter
        if (not dummyRead(m_sngDecryptedFile, 16, 0, count)) return false; // PhraseExtraInfo
        if (not dummyRead(m_sngDecryptedFile, 0, 0, count)) return false;  // LinkedDifficulty
        for (uint32_t i = 0; i < count; ++i)
        {
            uint32_t localCount;
            m_sngDecryptedFile.seek(m_sngDecryptedFile.pos() + 4);
            if (not dummyRead(m_sngDecryptedFile, 4, 0, localCount)) return false; // NdlPhrase
        }
        if (not dummyRead(m_sngDecryptedFile, 256, 0, count)) return false;          // Action
        if (not dummyRead(m_sngDecryptedFile, 260, 0, count)) return false;          // Event
        if (not dummyRead(m_sngDecryptedFile, 8, 0, count)) return false;            // Tone
        if (not dummyRead(m_sngDecryptedFile, 8, 0, count)) return false;            // DNA
        if (not dummyRead(m_sngDecryptedFile, 88, 0, count)) return false;           // Section
        if (not parseArrangements(m_sngDecryptedFile, m_arrangements)) return false; // Arrangement
        m_sngDecryptedFile.seek(m_sngDecryptedFile.pos() + 79);                      // Metadata
        if (not dummyRead(m_sngDecryptedFile, 2, 12, count)) return false;           // Tuning

        return m_sngDecryptedFile.pos() == m_sngDecryptedFile.size();
    }

    return false;
}

void SNG::setSngFile(const QString& newSngFileName)
{
    m_sngFile.setFileName(newSngFileName);
}

void SNG::setSngDecryptedFile(const QString& newSngDecryptedFileName)
{
    m_sngDecryptedFile.setFileName(newSngDecryptedFileName);
}
