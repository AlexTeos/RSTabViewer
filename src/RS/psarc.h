#ifndef PSARC_H__
#define PSARC_H__

#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QString>
#include <QVector>

#include "sng.h"

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

enum SngType
{
    Bass,
    Lead,
    Rhythm,
    Vocals,
    Combo,
    Showlights
};

class PSARC
{
    Q_PROPERTY(QString songName READ songName)
    Q_PROPERTY(QString artistName READ artistName)
    Q_PROPERTY(QString albumName READ albumName)
    Q_PROPERTY(int duration READ duration)
    Q_PROPERTY(int songYear READ songYear)
    Q_PROPERTY(QString songYear READ songYear)
    Q_PROPERTY(QString songYear READ songYear)
public:
    PSARC(const QString psarcDir);
    QString songName() const;
    QString artistName() const;
    QString albumName() const;
    int     duration() const;
    int     songYear() const;
    QString track() const;
    QString trackTeaser() const;

    //private:
    bool initializeAtributes();
    bool initializeSngs();
    bool initializeTracks();

    QJsonObject             m_songAtributes;
    QDir                    m_filesDir;
    QMap<SngType, RS::SNG>  m_sngs;
    QPair<QString, QString> m_tracks;
};
}

#endif // PSARC_H__
