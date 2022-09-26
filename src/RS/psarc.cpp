#include "psarc.h"

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include <ww2ogg.h>

#include "../src/RS/3rdparty/revorb/revorb.h"
#include "soundbank.h"

RS::PSARC::PSARC(const QString psarcDir) : m_filesDir(psarcDir)
{
    initializeAtributes();
    initializeSngs();
    initializeTracks();
    initializeImage();
}

QString RS::PSARC::songName() const
{
    if (m_songAtributes.contains("SongName") && m_songAtributes["SongName"].isString())
        return m_songAtributes["SongName"].toString();

    return "";
}

QString RS::PSARC::artistName() const
{
    if (m_songAtributes.contains("ArtistName") && m_songAtributes["ArtistName"].isString())
        return m_songAtributes["ArtistName"].toString();

    return "";
}

QString RS::PSARC::albumName() const
{
    if (m_songAtributes.contains("AlbumName") && m_songAtributes["AlbumName"].isString())
        return m_songAtributes["AlbumName"].toString();

    return "";
}

int RS::PSARC::duration() const
{
    if (m_songAtributes.contains("SongLength") && m_songAtributes["SongLength"].isDouble())
        return m_songAtributes["SongLength"].toDouble();

    return 0;
}

int RS::PSARC::songYear() const
{
    if (m_songAtributes.contains("SongYear") && m_songAtributes["SongYear"].isDouble())
        return m_songAtributes["SongYear"].toInt();

    return 0;
}

QString RS::PSARC::songBank() const
{
    if (m_songAtributes.contains("SongBank") && m_songAtributes["SongBank"].isString())
        return m_songAtributes["SongBank"].toString();

    return "";
}

QString RS::PSARC::previewBankPath() const
{
    if (m_songAtributes.contains("PreviewBankPath") && m_songAtributes["PreviewBankPath"].isString())
        return m_songAtributes["PreviewBankPath"].toString();

    return "";
}

QString RS::PSARC::track() const
{
    return m_tracks.second;
}

QString RS::PSARC::trackTeaser() const
{
    return m_tracks.first;
}

QString RS::PSARC::albumImage() const
{
    return m_albumImagePath;
}

QVariantList RS::PSARC::instruments() const
{
    QVariantList instruments;
    for (auto sngType : m_sngs.keys())
        switch (sngType)
        {
            case Bass:
            case Lead:
            case Rhythm:
                instruments.append(sngType);
            case Vocals:
            case Combo:
            case Showlights:
            default:
                break;
        }

    return instruments;
}

RS::SNG& RS::PSARC::sng(SngType type)
{
    return m_sngs[type];
}

bool RS::PSARC::initializeAtributes()
{
    QDir manifestDir(m_filesDir.path() + "/manifests");
    manifestDir.setPath(manifestDir.path() + "/" + manifestDir.entryList(QStringList() << "songs_*", QDir::Dirs)[0]);
    QStringList manifestsFileNames = manifestDir.entryList(QStringList() << "*.json", QDir::Files);
    QFile       manifest(manifestDir.path() + "/" + manifestsFileNames[0]);
    if (manifest.open(QIODevice::ReadOnly))
    {
        QByteArray arr = manifest.readAll();

        QJsonDocument replyJsonDocument(QJsonDocument::fromJson(arr));

        if (replyJsonDocument.isObject())
        {
            QJsonObject replyJsonObject = replyJsonDocument.object();
            if (replyJsonObject.contains("Entries") && replyJsonObject["Entries"].isObject())
            {
                QJsonObject entryObject = replyJsonObject["Entries"].toObject();
                if (entryObject.keys().size() > 0)
                {
                    entryObject = entryObject[entryObject.keys()[0]].toObject();
                    if (entryObject.contains("Attributes") && entryObject["Attributes"].isObject())
                    {
                        m_songAtributes = entryObject["Attributes"].toObject();
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool RS::PSARC::initializeSngs()
{
    QDir        sngsDir(m_filesDir.path() + "/songs/bin/generic");
    QStringList sngNames = sngsDir.entryList(QStringList() << "*.sng", QDir::Files);
    for (QString sngName : sngNames)
    {
        RS::SNG sng;
        if (not sngsDir.exists(sngName + "uc"))
        {
            // TODO: do only on demand
            if (not sng.decrypt(sngsDir.path() + "/" + sngName)) continue;
        }

        SngType sngType;
        if (sngName.contains("lead", Qt::CaseInsensitive))
            sngType = SngType::Lead;
        else if (sngName.contains("bass", Qt::CaseInsensitive))
            sngType = SngType::Bass;
        else if (sngName.contains("rhythm", Qt::CaseInsensitive))
            sngType = SngType::Rhythm;
        else
            continue;

        sng.setDecryptedFile(sngsDir.path() + "/" + sngName + "uc");
        m_sngs.insert(sngType, sng);
    }

    return m_sngs.size();
}

bool RS::PSARC::initializeTracks()
{
    // Main
    QFileInfo soundBankFile(m_filesDir.path() + "/audio/windows/" + songBank());
    if (not soundBankFile.exists()) return false;

    QString   wemId = SoundBank::getWemId(soundBankFile.filePath());
    QFileInfo wemFile(m_filesDir.path() + "/audio/windows/" + QString(wemId) + ".wem");
    if (not wemFile.exists()) return false;

    QString outFilePath = m_filesDir.path() + "/audio/windows/" + QString(wemId) + ".ogg";
    if (ww2ogg(wemFile.filePath().toStdString(), outFilePath.toStdString()))
    {
        if (revorb(outFilePath.toStdString()))
        {
            m_tracks.second = outFilePath;
        }
        else
            return false;
    }
    else
        return false;

    // Preview
    QFileInfo previewSoundBankFile(m_filesDir.path() + "/audio/windows/" + previewBankPath());
    if (not previewSoundBankFile.exists()) return false;

    QString   previewWmId = SoundBank::getWemId(previewSoundBankFile.filePath());
    QFileInfo previewWemFile(m_filesDir.path() + "/audio/windows/" + previewWmId + ".wem");
    if (not previewWemFile.exists()) return false;

    QString previewOutFilePath = m_filesDir.path() + "/audio/windows/" + previewWmId + ".ogg";
    if (ww2ogg(previewWemFile.filePath().toStdString(), previewOutFilePath.toStdString()))
    {
        if (revorb(previewOutFilePath.toStdString()))
        {
            m_tracks.first = previewOutFilePath;
        }
        else
            return false;
    }
    else
        return false;

    return true;
}

bool RS::PSARC::initializeImage()
{
    QDir        imagesDir(m_filesDir.path() + "/gfxassets/album_art");
    QStringList imageNames = imagesDir.entryList(QStringList() << "*_256.dds", QDir::Files);

    if (not imageNames.size()) return false;

    m_albumImagePath = imagesDir.path() + "/" + imageNames[0];

    return true;
}
