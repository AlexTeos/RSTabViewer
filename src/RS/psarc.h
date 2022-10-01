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
class PSARC
{
    Q_PROPERTY(QString songName READ songName)
    Q_PROPERTY(QString artistName READ artistName)
    Q_PROPERTY(QString albumName READ albumName)
    Q_PROPERTY(int duration READ duration)
    Q_PROPERTY(int songYear READ songYear)
    Q_PROPERTY(QString songYear READ songYear)
    Q_PROPERTY(QString songYear READ songYear)
    Q_PROPERTY(QString albumImage READ albumImage)
    Q_PROPERTY(QVariantList instruments READ instruments)
public:
    enum State
    {
        Uninitialized = 0,
        Initialized,
        Error
    };

    PSARC(const QString psarcDir);
    PSARC() : m_state(State::Uninitialized){};
    QString      songName() const;
    QString      artistName() const;
    QString      albumName() const;
    int          duration() const;
    int          songYear() const;
    QString      track() const;
    QString      trackTeaser() const;
    QString      albumImage() const;
    QVariantList arrangements() const;
    SNG&         sng(int index);
    State        state() const;

private:
    bool      initializeAtributes(const QString& filter = ".json");
    bool      initializeSngs();
    bool      initializeTracks();
    bool      initializeImage();
    SNG::Type clarifyTypeInManifest() const;

    QString songBank() const;
    QString previewBankPath() const;
    QString dlcKey() const;
    int     arrangementType() const;

    QJsonObject             m_songAtributes;
    QDir                    m_filesDir;
    QVector<SNG>            m_sngs;
    QPair<QString, QString> m_tracks;
    QString                 m_albumImagePath;
    State                   m_state;
};
}

#endif // PSARC_H__
