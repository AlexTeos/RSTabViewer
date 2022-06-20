#include "sng.h"

#include <QFileInfo>

#include "3rdparty/Rijndael/Rijndael.h"
#include "psarc.h"

static const unsigned char SngKeyPC[32] = {0xCB, 0x64, 0x8D, 0xF3, 0xD1, 0x2A, 0x16, 0xBF, 0x71, 0x70, 0x14,
                                           0x14, 0xE6, 0x96, 0x19, 0xEC, 0x17, 0x1C, 0xCA, 0x5D, 0x2A, 0x14,
                                           0x2E, 0x3E, 0x59, 0xDE, 0x7A, 0xDD, 0xA1, 0x8A, 0x3A, 0x30};

bool RS::SNG::decrypt(const QString& sngFileName)
{
    QFile sngFile(sngFileName);
    if (sngFile.open(QIODevice::ReadOnly))
    {
        if (((uint32_t*)sngFile.read(4).constData())[0] == 0x4a)
        {
            if (((uint32_t*)sngFile.read(4).constData())[0] == 0x03)
            {
                qsizetype  offset      = 8;
                uint64_t   writeOffset = 0;
                uint8_t    blockLength = 16;
                QByteArray decryptedSng;
                decryptedSng.resize(sngFile.size() + 32);
                char iv[16];
                for (int i = 0; i < 16; i++)
                {
                    sngFile.read(&iv[i], 1);
                }
                CRijndael rijndael;

                do
                {
                    rijndael.MakeKey(SngKeyPC, (unsigned char*)iv, 32, blockLength);
                    rijndael.Decrypt((unsigned char*)sngFile.read(blockLength).constData(),
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
                } while (offset < sngFile.size());
                sngFile.close();

                QByteArray uncompressedData;
                uLongf     uncompressedSize = ((uint32_t*)decryptedSng.constData())[0];
                decryptedSng.remove(0, 4);
                uncompress(uncompressedData, uncompressedSize, decryptedSng);

                QFile sngDecryptedFile(sngFile.fileName().insert(
                    sngFile.fileName().length() - QFileInfo(sngFile).suffix().length() - 1, "_uncompressed"));

                if (sngDecryptedFile.open(QIODevice::WriteOnly))
                {
                    sngDecryptedFile.write(uncompressedData);
                    sngDecryptedFile.close();
                    return true;
                }
            }
        }
    }
    return false;
}

bool RS::SNG::dummyRead(QIODevice& input, const qint64& structureSize, const qint64& additionalSize, uint32_t& count)
{
    qint64 fileSize = input.size();
    count           = *((uint32_t*)input.read(4).constData());
    input.seek(input.pos() + additionalSize + structureSize * count);
    return input.pos() <= fileSize;
}

bool RS::SNG::parse(const QString& decryptedSngFileName)
{
    QFile sngDecryptedFile(decryptedSngFileName);
    if (sngDecryptedFile.open(QIODevice::ReadOnly))
    {
        uint32_t count;

        if (not dummyRead(sngDecryptedFile, 16, 0, count)) return false;   // Beat
        if (not dummyRead(sngDecryptedFile, 44, 0, count)) return false;   // Phrase
        if (not parseChords(sngDecryptedFile, m_chords)) return false;     // Chord
        if (not dummyRead(sngDecryptedFile, 2376, 0, count)) return false; // ChordsNote
        if (not dummyRead(sngDecryptedFile, 60, 0, count)) return false;   // Vocals
        if (count > 0)
        {
            uint32_t localCount;
            if (not dummyRead(sngDecryptedFile, 32, 0, localCount)) return false;  // Header
            if (not dummyRead(sngDecryptedFile, 144, 0, localCount)) return false; // Texture
            if (not dummyRead(sngDecryptedFile, 44, 0, localCount)) return false;  // Definition
        }
        if (not dummyRead(sngDecryptedFile, 24, 0, count)) return false; // PhrasesIter
        if (not dummyRead(sngDecryptedFile, 16, 0, count)) return false; // PhraseExtraInfo
        if (not dummyRead(sngDecryptedFile, 0, 0, count)) return false;  // LinkedDifficulty
        for (uint32_t i = 0; i < count; ++i)
        {
            uint32_t localCount;
            sngDecryptedFile.seek(sngDecryptedFile.pos() + 4);
            if (not dummyRead(sngDecryptedFile, 4, 0, localCount)) return false; // NdlPhrase
        }
        if (not dummyRead(sngDecryptedFile, 256, 0, count)) return false;          // Action
        if (not dummyRead(sngDecryptedFile, 260, 0, count)) return false;          // Event
        if (not dummyRead(sngDecryptedFile, 8, 0, count)) return false;            // Tone
        if (not dummyRead(sngDecryptedFile, 8, 0, count)) return false;            // DNA
        if (not dummyRead(sngDecryptedFile, 88, 0, count)) return false;           // Section
        if (not parseArrangements(sngDecryptedFile, m_arrangements)) return false; // Arrangement
        if (not parseMetadata(sngDecryptedFile, m_metadata)) return false;         // Metadata

        return sngDecryptedFile.pos() == sngDecryptedFile.size();
    }

    return false;
}
