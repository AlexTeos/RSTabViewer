#include "sng.h"

#include <QDebug>>
#include <QFileInfo>

#include "3rdparty/Rijndael/Rijndael.h"

namespace RS
{
static const unsigned char SngKeyPC[32] = {0xCB, 0x64, 0x8D, 0xF3, 0xD1, 0x2A, 0x16, 0xBF, 0x71, 0x70, 0x14,
                                           0x14, 0xE6, 0x96, 0x19, 0xEC, 0x17, 0x1C, 0xCA, 0x5D, 0x2A, 0x14,
                                           0x2E, 0x3E, 0x59, 0xDE, 0x7A, 0xDD, 0xA1, 0x8A, 0x3A, 0x30};

SNG::SNG(Type type, const QString& file) : m_type(type), m_file(file) {}

bool SNG::decrypt(const QString& sngFileName) const
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

                QFile sngDecryptedFile(sngFile.fileName() + "uc");

                if (sngDecryptedFile.open(QIODevice::WriteOnly))
                {
                    sngDecryptedFile.write(uncompressedData);
                    sngDecryptedFile.close();
                    return true;
                }
            }
        }
    }

    qCritical() << "Can't decrypt file " << sngFileName;
    return false;
}

bool SNG::dummyRead(QIODevice& input, const qint64& structureSize, const qint64& additionalSize, uint32_t& count) const
{
    qint64 fileSize = input.size();
    count           = *((uint32_t*)input.read(4).constData());
    input.seek(input.pos() + additionalSize + structureSize * count);
    return input.pos() <= fileSize;
}

bool SNG::parse(const QString& decryptedSngFileName) const
{
    QFile sngDecryptedFile(decryptedSngFileName);
    if (sngDecryptedFile.open(QIODevice::ReadOnly))
    {
        uint32_t count;

        if (not dummyRead(sngDecryptedFile, 16, 0, count)) // Beat
        {
            qCritical() << "Can't parse Beat in file " << sngDecryptedFile;
            return false;
        };
        if (not dummyRead(sngDecryptedFile, 44, 0, count)) // Phrase
        {
            qCritical() << "Can't parse Phrase in file " << sngDecryptedFile;
            return false;
        };
        if (not parseChords(sngDecryptedFile, m_chords)) // Chord
        {
            qCritical() << "Can't parse Chords in file " << sngDecryptedFile;
            return false;
        };
        if (not parseChordNotes(sngDecryptedFile, m_chordNotes)) // ChordsNote
        {
            qCritical() << "Can't parse ChordNotes in file " << sngDecryptedFile;
            return false;
        };
        if (not dummyRead(sngDecryptedFile, 60, 0, count)) // Vocals
        {
            qCritical() << "Can't parse Vocals in file " << sngDecryptedFile;
            return false;
        };
        if (count > 0)
        {
            uint32_t localCount;
            if (not dummyRead(sngDecryptedFile, 32, 0, localCount)) // Header
            {
                qCritical() << "Can't parse Header in file " << sngDecryptedFile;
                return false;
            };
            if (not dummyRead(sngDecryptedFile, 144, 0, localCount)) // Texture
            {
                qCritical() << "Can't parse Texture in file " << sngDecryptedFile;
                return false;
            };
            if (not dummyRead(sngDecryptedFile, 44, 0, localCount)) // Definition
            {
                qCritical() << "Can't parse Definition in file " << sngDecryptedFile;
                return false;
            };
        }
        if (not dummyRead(sngDecryptedFile, 24, 0, count)) // PhrasesIter
        {
            qCritical() << "Can't parse PhrasesIter in file " << sngDecryptedFile;
            return false;
        };
        if (not dummyRead(sngDecryptedFile, 16, 0, count)) // PhraseExtraInfo
        {
            qCritical() << "Can't parse PhraseExtraInfo in file " << sngDecryptedFile;
            return false;
        };
        if (not dummyRead(sngDecryptedFile, 0, 0, count)) // LinkedDifficulty
        {
            qCritical() << "Can't parse LinkedDifficulty in file " << sngDecryptedFile;
            return false;
        };
        for (uint32_t i = 0; i < count; ++i)
        {
            uint32_t localCount;
            sngDecryptedFile.seek(sngDecryptedFile.pos() + 4);
            if (not dummyRead(sngDecryptedFile, 4, 0, localCount)) // NdlPhrase
            {
                qCritical() << "Can't parse NdlPhrase in file " << sngDecryptedFile;
                return false;
            };
        }
        if (not dummyRead(sngDecryptedFile, 256, 0, count)) // Action
        {
            qCritical() << "Can't parse Action in file " << sngDecryptedFile;
            return false;
        };
        if (not dummyRead(sngDecryptedFile, 260, 0, count)) // Event
        {
            qCritical() << "Can't parse Event in file " << sngDecryptedFile;
            return false;
        };
        if (not dummyRead(sngDecryptedFile, 8, 0, count)) // Tone
        {
            qCritical() << "Can't parse Tone in file " << sngDecryptedFile;
            return false;
        };
        if (not dummyRead(sngDecryptedFile, 8, 0, count)) // DNA
        {
            qCritical() << "Can't parse DNA in file " << sngDecryptedFile;
            return false;
        };
        if (not dummyRead(sngDecryptedFile, 88, 0, count)) // Section
        {
            qCritical() << "Can't parse Section in file " << sngDecryptedFile;
            return false;
        };
        if (not parseArrangements(sngDecryptedFile, m_arrangements)) // Arrangement
        {
            qCritical() << "Can't parse Arrangements in file " << sngDecryptedFile;
            return false;
        };
        if (not parseMetadata(sngDecryptedFile, m_metadata)) // Metadata
        {
            qCritical() << "Can't parse Metadata in file " << sngDecryptedFile;
            return false;
        }

        return sngDecryptedFile.pos() == sngDecryptedFile.size();
    }

    qCritical() << "Can't parse file " << decryptedSngFileName;
    return false;
}

const QVector<Arrangement>& SNG::arrangements() const
{
    initialize();

    return m_arrangements;
}

const Metadata& SNG::metadata() const
{
    initialize();
    return m_metadata;
}

const QVector<Chord>& SNG::chords() const
{
    initialize();

    return m_chords;
}

const QVector<ChordNote>& SNG::chordNotes() const
{
    initialize();

    return m_chordNotes;
}

bool SNG::initialize() const
{
    if (not m_initialized)
    {
        if (decrypt(m_file)) m_initialized = parse(m_file + "uc");
    }

    return m_initialized;
}

SNG::Type SNG::type() const
{
    return m_type;
}
}
