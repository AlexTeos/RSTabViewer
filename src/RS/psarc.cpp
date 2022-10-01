#include "psarc.h"

#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonObject>

#include <ww2ogg.h>

#include "../src/RS/3rdparty/revorb/revorb.h"
#include "soundbank.h"

namespace RS
{
PSARC::PSARC(const QString psarcDir) : m_filesDir(psarcDir), m_state(State::Uninitialized)
{
    if (initializeAtributes())
        if (initializeSngs())
            if (initializeTracks())
                if (initializeImage())
                {
                    m_state = State::Initialized;
                    return;
                }

    m_state = State::Uninitialized;
}

QString PSARC::songName() const
{
    if (m_songAtributes.contains("SongName") && m_songAtributes["SongName"].isString())
        return m_songAtributes["SongName"].toString();

    return "";
}

QString PSARC::artistName() const
{
    if (m_songAtributes.contains("ArtistName") && m_songAtributes["ArtistName"].isString())
        return m_songAtributes["ArtistName"].toString();

    return "";
}

QString PSARC::albumName() const
{
    if (m_songAtributes.contains("AlbumName") && m_songAtributes["AlbumName"].isString())
        return m_songAtributes["AlbumName"].toString();

    return "";
}

int PSARC::duration() const
{
    if (m_songAtributes.contains("SongLength") && m_songAtributes["SongLength"].isDouble())
        return m_songAtributes["SongLength"].toDouble();

    return 0;
}

int PSARC::songYear() const
{
    if (m_songAtributes.contains("SongYear") && m_songAtributes["SongYear"].isDouble())
        return m_songAtributes["SongYear"].toInt();

    return 0;
}

QString PSARC::songBank() const
{
    if (m_songAtributes.contains("SongBank") && m_songAtributes["SongBank"].isString())
        return m_songAtributes["SongBank"].toString();

    return "";
}

QString PSARC::previewBankPath() const
{
    if (m_songAtributes.contains("PreviewBankPath") && m_songAtributes["PreviewBankPath"].isString())
        return m_songAtributes["PreviewBankPath"].toString();

    return "";
}

QString PSARC::track() const
{
    return m_tracks.second;
}

QString PSARC::trackTeaser() const
{
    return m_tracks.first;
}

QString PSARC::albumImage() const
{
    return m_albumImagePath;
}

QVariantList PSARC::arrangements() const
{
    QVariantList arrangements;
    for (const auto& sng : m_sngs)
        arrangements.append(sng.type());

    return arrangements;
}

SNG& PSARC::sng(int index)
{
    //TODO: FIX
    m_sngs[index].metadata();
    return m_sngs[index];
}

PSARC::State PSARC::state() const
{
    return m_state;
}

QString PSARC::dlcKey() const
{
    if (m_songAtributes.contains("AlbumArt") && m_songAtributes["AlbumArt"].isString())
        return m_songAtributes["AlbumArt"].toString().remove((QRegExp(".*_")));

    return "";
}

int PSARC::arrangementType() const
{
    if (m_songAtributes.contains("ArrangementType") && m_songAtributes["ArrangementType"].isDouble())
        return m_songAtributes["ArrangementType"].toInt();

    return 0;
}

bool PSARC::initializeAtributes(const QStringList& filters)
{
    QDirIterator it(
        m_filesDir.path() + "/manifests", QStringList() << filters, QDir::Files, QDirIterator::Subdirectories);

    if (it.hasNext())
    {
        QFile manifest(it.next());

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
    }

    return false;
}

SNG::Type PSARC::clarifyTypeInManifest() const
{
    switch (arrangementType())
    {
        case 0:
            return SNG::Type::Lead;
            break;
        case 1:
            return SNG::Type::Rhythm;
            break;
        case 2:
            return SNG::Type::Combo;
            break;
        case 3:
        default:
            return SNG::Type::Bass;
    }
}

bool PSARC::initializeSngs()
{
    QDir        sngsDir(m_filesDir.path() + "/songs/bin/generic");
    QStringList sngNames = sngsDir.entryList(QStringList() << "*.sng", QDir::Files);
    for (QString sngName : sngNames)
    {
        SNG::Type sngType;
        if (sngName.contains("lead", Qt::CaseInsensitive))
            sngType = SNG::Type::Lead;
        else if (sngName.contains("bass", Qt::CaseInsensitive))
            sngType = SNG::Type::Bass;
        else if (sngName.contains("rhythm", Qt::CaseInsensitive))
            sngType = SNG::Type::Rhythm;
        else if (sngName.contains("combo", Qt::CaseInsensitive))
        {
            QString jsonFile = sngName;
            jsonFile.chop(3);
            jsonFile += ("json");

            initializeAtributes({"*" + jsonFile});
            sngType = clarifyTypeInManifest();
        }
        else
            continue;

        m_sngs.push_back(SNG(sngType, sngsDir.path() + "/" + sngName));
    }

    return m_sngs.size();
}

bool PSARC::initializeTracks()
{
    // Main
    QFileInfo soundBankFile(m_filesDir.path() + "/audio/windows/" + songBank());
    if (not soundBankFile.exists()) return false;

    QString wemId = SoundBank::getWemId(soundBankFile.filePath());

    QString outFilePath = m_filesDir.path() + "/audio/windows/" + QString(wemId) + ".ogg";
    if (QDir().exists(outFilePath))
    {
        m_tracks.second = outFilePath;
    }
    else
    {
        QFileInfo wemFile(m_filesDir.path() + "/../audio/windows/" + QString(wemId) + ".wem");
        if (not wemFile.exists()) return false;

        if (ww2ogg(wemFile.filePath().toStdString(), outFilePath.toStdString()))
        {
            if (revorb(outFilePath.toStdString()))
                m_tracks.second = outFilePath;
            else
                return false;
        }
        else
            return false;
    }

    // Preview
    QFileInfo previewSoundBankFile(m_filesDir.path() + "/audio/windows/" + previewBankPath());
    if (not previewSoundBankFile.exists()) return false;

    QString previewWmId = SoundBank::getWemId(previewSoundBankFile.filePath());

    QString previewOutFilePath = m_filesDir.path() + "/audio/windows/" + previewWmId + ".ogg";
    if (QDir().exists(previewOutFilePath))
    {
        m_tracks.first = previewOutFilePath;
    }
    else
    {
        QFileInfo previewWemFile(m_filesDir.path() + "/../audio/windows/" + previewWmId + ".wem");
        if (not previewWemFile.exists()) return false;

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
    }

    return true;
}

bool PSARC::initializeImage()
{
    QFileInfo albumImageFile(m_filesDir.path() + "/gfxassets/album_art/album_" + dlcKey() + "_256.dds");

    if (not albumImageFile.exists()) return false;

    m_albumImagePath = albumImageFile.filePath();

    return true;
}
}
