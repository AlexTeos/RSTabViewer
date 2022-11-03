#include "musiclibrary.h"

#include <QDirIterator>
#include <future>
#include <thread>

#include "RS/psarcarchive.h"

MusicLibrary::MusicLibrary(const QDir& libDir, QObject* parent) : QAbstractListModel(parent), m_libraryDir(libDir) {}

int MusicLibrary::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return m_psarcs.size();
}

QVariant MusicLibrary::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    switch (role)
    {
        case SongNameRole:
            return m_psarcs[index.row()].songName();
        case ArtistNameRole:
            return m_psarcs[index.row()].artistName();
        case AlbumNameRole:
            return m_psarcs[index.row()].albumName();
        case DurationRole:
            return m_psarcs[index.row()].duration();
        case SongYearRole:
            return m_psarcs[index.row()].songYear();
        case TrackRole:
            return m_psarcs[index.row()].track();
        case TrackTeaserRole:
            return m_psarcs[index.row()].trackTeaser();
        case AlbumImageRole:
            return m_psarcs[index.row()].albumImage();
        case ArrangementsRole:
            return m_psarcs[index.row()].arrangements();
    }

    return QVariant();
}

Qt::ItemFlags MusicLibrary::flags(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return Qt::NoItemFlags;
}

QHash<int, QByteArray> MusicLibrary::roleNames() const
{
    QHash<int, QByteArray> names;
    names[SongNameRole]     = "songName";
    names[ArtistNameRole]   = "artistName";
    names[AlbumNameRole]    = "albumName";
    names[DurationRole]     = "duration";
    names[SongYearRole]     = "songYear";
    names[TablatureRole]    = "tablature";
    names[TrackRole]        = "track";
    names[TrackTeaserRole]  = "trackTeaser";
    names[AlbumImageRole]   = "albumImage";
    names[ArrangementsRole] = "arrangements";

    return names;
}

void MusicLibrary::unarchiveNArchives(QStringList::ConstIterator nameIter, int count) const
{
    for (int i = 0; i < count; ++i, ++nameIter)
    {
        QString archiveFullName = m_libraryDir.path() + "/" + *nameIter;
        QString unpackPath      = m_libraryDir.path() + "/" + QFileInfo(archiveFullName).baseName();
        qDebug() << "Unzip psarc: " << *nameIter;
        if (*nameIter == "rs1compatibilitydlc_p.psarc" or *nameIter == "songs.psarc")
        {
            QMutexLocker locker(&m_songDlc);
            QDir().mkdir(unpackPath);
            unpackPath = m_libraryDir.path() + "/songsanddlc";
            RS::PSARCArchive::unarchive(archiveFullName, unpackPath);
        }
        else
            RS::PSARCArchive::unarchive(archiveFullName, unpackPath);
    }
}

void MusicLibrary::loadNArchives(QVector<RS::PSARC>::Iterator psarcIter,
                                 QStringList::ConstIterator   nameIter,
                                 int                          count) const
{
    for (int i = 0; i < count; ++i, ++psarcIter, ++nameIter)
    {
        // TODO: is it a best way?
        qDebug() << "Load psarc: " << *nameIter;
        // TODO: check psarc
        RS::PSARC psarc(*nameIter);
        *psarcIter = std::move(psarc);
    }
}

void MusicLibrary::load()
{
    if (not m_libraryDir.exists())
    {
        QDir().mkdir(m_libraryDir.path());
        qCritical() << "Library directory doesn't exist";
        return;
    }

    beginResetModel();

    // TODO: use QFuture
    // TODO: threadCount = 1 in debug
    qint16 threadCount = std::thread::hardware_concurrency();
    qInfo() << "Thread count: " << threadCount;
    std::vector<std::future<void>> results;

    // Unarchive
    qInfo() << "Start unzipping";
    QStringList archiveNames = m_libraryDir.entryList(QStringList() << "*.psarc", QDir::Files);
    QStringList unarchivedArchiveNames;
    for (const auto& archive : archiveNames)
    {
        QString unarchiveDest =
            m_libraryDir.path() + "/" + QFileInfo(m_libraryDir.path() + "/" + archive).completeBaseName();
        if (not QDir(unarchiveDest).exists())
        {
            unarchivedArchiveNames += archive;
            qInfo() << "Archive to unzip: " << archive;
        };
    }
    qInfo() << "Archive to unzip: " << unarchivedArchiveNames.size();

    qsizetype archivesPerThread =
        unarchivedArchiveNames.size() >= threadCount ? unarchivedArchiveNames.size() / threadCount : 1;

    QStringList::ConstIterator unarchivedArchiveNamesIter = unarchivedArchiveNames.cbegin();
    for (qsizetype i = 0; i < unarchivedArchiveNames.size();
         unarchivedArchiveNamesIter += archivesPerThread, i += archivesPerThread)
    {
        archivesPerThread = qMin(unarchivedArchiveNames.length() - i, archivesPerThread);
        results.push_back(
            std::async(&MusicLibrary::unarchiveNArchives, this, unarchivedArchiveNamesIter, archivesPerThread));
    }
    for (auto& result : results)
    {
        result.get();
    }

    // Load
    qInfo() << "Start sng loading";
    QStringList songPaths;

    QDirIterator psarcLevel(m_libraryDir.path(), QDir::Dirs | QDir::NoDotAndDotDot);
    while (psarcLevel.hasNext())
    {
        QDirIterator songsLevel(psarcLevel.next(), QDir::Dirs | QDir::NoDotAndDotDot);
        while (songsLevel.hasNext())
        {
            songsLevel.next();
            if (not songsLevel.fileName().contains("audio"))
            {
                qInfo() << "Path to load: " << songsLevel.filePath();
                songPaths.push_back(songsLevel.filePath());
            }
        }
    }

    qInfo() << "Paths to load: " << songPaths.length();
    m_psarcs.resize(songPaths.length());

    qsizetype songsPerThread = songPaths.size() >= threadCount ? songPaths.size() / threadCount : 1;
    results.clear();

    QVector<RS::PSARC>::Iterator psarcIter = m_psarcs.begin();
    QStringList::ConstIterator   nameIter  = songPaths.cbegin();
    for (qsizetype i = 0; i < songPaths.size();
         psarcIter += songsPerThread, nameIter += songsPerThread, i += songsPerThread)
    {
        songsPerThread = qMin(songPaths.length() - i, songsPerThread);
        results.push_back(std::async(&MusicLibrary::loadNArchives, this, psarcIter, nameIter, songsPerThread));
    }
    for (auto& result : results)
    {
        result.get();
    }

    QVector<RS::PSARC>::Iterator iter = m_psarcs.begin();
    while (iter != m_psarcs.end())
    {
        if (iter->state() != RS::PSARC::State::Initialized)
            iter = m_psarcs.erase(iter);
        else
            ++iter;
    }

    endResetModel();
}

Tablature* MusicLibrary::tablature()
{
    return &m_tablature;
}

void MusicLibrary::setTablature(int psarcIndex, int sngIndex)
{
    qDebug() << "Set sng to tablature; song name: " << m_psarcs[psarcIndex].songName()
             << " instrument id: " << sngIndex;
    m_tablature.setSNG(m_psarcs[psarcIndex].sng(sngIndex));
}
