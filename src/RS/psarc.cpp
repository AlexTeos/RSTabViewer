#include "psarc.h"

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include <ww2ogg.h>

#include "../src/RS/3rdparty/revorb/revorb.h"
#include "3rdparty/Rijndael/Rijndael.h"
#include "common.h"

static const unsigned char PsarcKey[32] = {0xC5, 0x3D, 0xB2, 0x38, 0x70, 0xA1, 0xA2, 0xF7, 0x1C, 0xAE, 0x64,
                                           0x06, 0x1F, 0xDD, 0x0E, 0x11, 0x57, 0x30, 0x9D, 0xC8, 0x52, 0x04,
                                           0xD4, 0xC5, 0xBF, 0xDF, 0x25, 0x09, 0x0D, 0xF2, 0x57, 0x2C};

bool RS::PSARCArchive::inflateEntry(uint32_t&            entry,
                                    QVector<uint32_t>&   zBlocks,
                                    uint32_t&            cBlockSize,
                                    QString              fileName,
                                    QFile&               file,
                                    QVector<PSARCEntry>& entries)
{
    if (entry == 0) fileName = "psarc.temp";

    QDir().mkpath(QFileInfo(fileName).path());

    QFile stream(fileName);
    if (stream.open(QIODevice::WriteOnly))
    {
        if (entries[entry].m_length != 0)
        {
            file.seek(entries[entry].m_zOffset);
            uint32_t zIndex = entries[entry].m_zIndex;
            do
            {
                if (zBlocks[zIndex] == 0)
                {
                    stream.write(file.read(cBlockSize), cBlockSize);
                }
                else
                {
                    uint16_t   isGzipped      = READ_BE_UINT16((uint8_t*)file.peek(2).constData());
                    QByteArray compressedData = file.read(zBlocks[zIndex]);
                    if (isGzipped == 0x78da)
                    {
                        QByteArray uncompressedData;
                        uncompress(uncompressedData,
                                   qMin(entries[entry].m_length - (zIndex - entries[entry].m_zIndex) * cBlockSize,
                                        (uint64_t)cBlockSize),
                                   compressedData);
                        stream.write(uncompressedData);
                    }
                    else
                        stream.write(compressedData, zBlocks[zIndex]);
                }
                zIndex++;
                if (zIndex >= zBlocks.size())
                {
                    // TODO: handle somehow
                    qWarning() << "Error during uncompression [" << fileName << "]";
                    break;
                }
            } while (stream.pos() < entries[entry].m_length);
        }

        if (entry == 0)
        {
            stream.close();

            QFile reader(fileName);
            reader.open(QIODevice::ReadOnly);

            entries[0].m_name = (char*)"NamesBlock.bin";
            for (uint32_t i = 1; i < entries.size(); i++)
            {
                entries[i].m_name = reader.readLine();
                entries[i].m_name.remove(entries[i].m_name.length() - 1, 1);
                entries[i].m_name.replace("/", "\\");
            }

            reader.close();
            stream.remove();
        }
        else
        {
            stream.close();
            return false;
        }
        return true;
    }
    return false;
}

bool RS::PSARCArchive::unarchive(const QString& archiveName, const QString& unpackDir)
{
    QVector<PSARCEntry> entries;

    QFile psarcFile(archiveName);
    if (psarcFile.open(QIODevice::ReadOnly))
    {
        if (READ_BE_UINT32((uint8_t*)psarcFile.read(4).constData()) == kPSARCMagicNumber)
        {
            psarcFile.seek(8);
            if (READ_BE_UINT32((uint8_t*)psarcFile.read(4).constData()) == 0x7a6c6962)
            {
                uint32_t zSize = READ_BE_UINT32((uint8_t*)psarcFile.read(4).constData());
                psarcFile.seek(headerSize);
                uint32_t _numEntries = READ_BE_UINT32((uint8_t*)psarcFile.read(4).constData());
                psarcFile.seek(24);
                uint32_t cBlockSize = READ_BE_UINT32((uint8_t*)psarcFile.read(4).constData());

                uint8_t  zType = 1;
                uint32_t i     = 256;
                do
                {
                    i *= 256;
                    zType = (uint8_t)(zType + 1);
                } while (i < cBlockSize);

                psarcFile.seek(32);

                QByteArray decryptedToc;
                decryptedToc.resize(zSize);
                CRijndael rijndael;

                rijndael.MakeKey(PsarcKey, (unsigned char*)CRijndael::sm_chain0, 32, 16);
                rijndael.Decrypt((unsigned char*)psarcFile.read(zSize - 32).constData(),
                                 (unsigned char*)decryptedToc.data(),
                                 zSize & ~31,
                                 CRijndael::CFB);

                qsizetype decryptedTocOffset = 0;
                entries.resize(_numEntries);
                for (uint32_t i = 0; i < entries.size(); i++)
                {
                    decryptedTocOffset += 16;

                    entries[i].m_id = i;

                    entries[i].m_zIndex = READ_BE_UINT32((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                    decryptedTocOffset += 4;

                    entries[i].m_length = READ_BE_INT40((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                    decryptedTocOffset += 5;

                    entries[i].m_zOffset = READ_BE_INT40((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                    decryptedTocOffset += 5;
                }

                uint32_t          numBlocks = (zSize - decryptedTocOffset - headerSize) / zType;
                QVector<uint32_t> zBlocks;
                zBlocks.resize(numBlocks);
                for (uint32_t i = 0; i < numBlocks; i++)
                {
                    switch (zType)
                    {
                        case 2:
                            zBlocks[i] = READ_BE_UINT16((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                            decryptedTocOffset += 2;
                            break;

                        case 3:
                            zBlocks[i] = READ_BE_INT24((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                            decryptedTocOffset += 3;
                            break;

                        case 4:
                            zBlocks[i] = READ_BE_UINT32((uint8_t*)decryptedToc.constData() + decryptedTocOffset);
                            decryptedTocOffset += 4;
                            break;
                    }
                }

                for (uint32_t i = 0; i < entries.size(); i++)
                {
                    inflateEntry(i, zBlocks, cBlockSize, unpackDir + "\\" + entries[i].m_name, psarcFile, entries);
                }

                return true;
            }
        }

        psarcFile.close();
    }
    return false;
}

RS::PSARC::PSARC(const QString psarcDir) : m_filesDir(psarcDir)
{
    initializeAtributes();
    initializeSngs();
    initializeTracks();
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

QString RS::PSARC::track() const
{
    return m_tracks.second;
}

QString RS::PSARC::trackTeaser() const
{
    return m_tracks.first;
}

bool RS::PSARC::initializeAtributes()
{
    QDir manifestDir(m_filesDir.path() + "\\manifests");
    manifestDir.setPath(manifestDir.path() + "\\" + manifestDir.entryList(QStringList() << "songs_*", QDir::Dirs)[0]);
    QStringList manifestsFileNames = manifestDir.entryList(QStringList() << "*.json", QDir::Files);
    QFile       manifest(manifestDir.path() + "\\" + manifestsFileNames[0]);
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
    QDir        sngsDir(m_filesDir.path() + "\\songs\\bin\\generic");
    QStringList sngNames = sngsDir.entryList(QStringList() << "*.sng", QDir::Files);
    for (QString sngName : sngNames)
    {
        RS::SNG sng;
        if (not sngsDir.exists(sngName + "uc"))
        {
            // TODO: do only on demand
            if (not sng.decrypt(sngsDir.path() + "\\" + sngName)) continue;
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

        if (sng.parse(sngsDir.path() + "\\" + sngName + "uc"))
        {
            m_sngs.insert(sngType, sng);
        }
    }

    return m_sngs.size();
}

bool RS::PSARC::initializeTracks()
{
    QVector<QFileInfo> tracks;
    QDir               tracksDir(m_filesDir.path() + "\\audio\\windows");
    QStringList        trackNames = tracksDir.entryList(QStringList() << "*.wem", QDir::Files);
    for (QString trackName : trackNames)
    {
        QString filePath = tracksDir.path() + "\\" + trackName;
        trackName.replace(QStringLiteral(".wem"), QStringLiteral(".ogg"), Qt::CaseInsensitive);
        QString outFilePath = tracksDir.path() + "\\" + trackName;

        if (not tracksDir.exists(trackName))
        {
            // TODO: do only on demand
            // TODO: remove wem
            if (ww2ogg(filePath.toStdString(), outFilePath.toStdString()))
                if (revorb(outFilePath.toStdString())) tracks.append(QFileInfo(outFilePath));
        }
        else
        {
            tracks.append(QFileInfo(outFilePath));
        }
    }

    if (not tracks.size()) return false;

    if (tracks.size() > 1)
    {
        sort(tracks.begin(), tracks.end(), [](const QFileInfo& a, const QFileInfo& b) { return a.size() < b.size(); });
        m_tracks.first  = tracks[0].filePath();
        m_tracks.second = tracks[1].filePath();
    }
    else
    {
        m_tracks.first  = tracks[0].filePath();
        m_tracks.second = tracks[0].filePath();
    }

    return true;
}
