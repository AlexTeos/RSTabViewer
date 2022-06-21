#ifndef PSARC_H__
#define PSARC_H__

#include <QFile>
#include <QString>
#include <QVector>

namespace RS
{
class PSARCArchive
{
public:
    PSARCArchive() = delete;
    static bool unarchive(const QString& psarcFileName, const QString& unpackDir);

private:
    struct PSARCEntry
    {
        int32_t  m_id;
        uint64_t m_length;
        QString  m_name;
        uint32_t m_zIndex;
        uint64_t m_zOffset;
    };

    static const uint32_t headerSize        = 20;
    static const uint32_t kPSARCMagicNumber = 0x50534152;

    static bool inflateEntry(uint32_t&            entry,
                             QVector<uint32_t>&   zBlocks,
                             uint32_t&            cBlockSize,
                             QString              fileName,
                             QFile&               file,
                             QVector<PSARCEntry>& entries);
};
}

#endif // PSARC_H__
