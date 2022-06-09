#ifndef PSARC_H__
#define PSARC_H__

#include <QFile>
#include <QString>
#include <QVector>

struct PSARCEntry
{
    int32_t  m_id;
    uint64_t m_length;
    QString  m_name;
    uint32_t m_zIndex;
    uint64_t m_zOffset;
};

namespace RS
{
class PSARC
{
public:
    PSARC(){};
    PSARC(const QString& archiveName);
    bool unarchive();

    void setPsarcFile(const QString& newPsarcFileName);

private:
    const uint32_t headerSize        = 20;
    const uint32_t kPSARCMagicNumber = 0x50534152;

    bool inflateEntry(uint32_t& entry, QVector<uint32_t>& zBlocks, uint32_t& cBlockSize, QString fileName, QFile& file);

    QFile               m_psarcFile;
    QVector<PSARCEntry> m_entries;
};
}

#endif // PSARC_H__
