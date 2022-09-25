#include "musiclibrary.h"

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
        case InstrumentsRole:
            return m_psarcs[index.row()].instruments();
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
    names[SongNameRole]    = "songName";
    names[ArtistNameRole]  = "artistName";
    names[AlbumNameRole]   = "albumName";
    names[DurationRole]    = "duration";
    names[SongYearRole]    = "songYear";
    names[TablatureRole]   = "tablature";
    names[TrackRole]       = "track";
    names[TrackTeaserRole] = "trackTeaser";
    names[AlbumImageRole]  = "albumImage";
    names[InstrumentsRole] = "instruments";

    return names;
}

#include <QElapsedTimer>
#include <future>
#include <thread>

void MusicLibrary::loadNArchives(QVector<RS::PSARC>::Iterator psarcIter,
                                 QStringList::ConstIterator   nameIter,
                                 int                          count) const
{
    for (int i = 0; i < count; ++i, ++psarcIter, ++nameIter)
    {
        QString archiveFullName = m_libraryDir.path() + "/" + *nameIter;
        QString unpackPath      = m_libraryDir.path() + "/" + QFileInfo(archiveFullName).baseName();
        if (not QDir(QFileInfo(archiveFullName).completeBaseName()).exists())
            RS::PSARCArchive::unarchive(archiveFullName, unpackPath);
        // TODO: is it a best way?
        RS::PSARC psarc(unpackPath);
        *psarcIter = std::move(psarc);
    }
}

void MusicLibrary::load()
{
    if (not m_libraryDir.exists())
    {
        QDir().mkdir(m_libraryDir.path());
        return;
    }

    beginResetModel();
    QStringList archiveNames = m_libraryDir.entryList(QStringList() << "*.psarc", QDir::Files);
    m_psarcs.resize(archiveNames.length());

    // TODO: use QFuture
    qint16    threadCount = std::thread::hardware_concurrency();
    qsizetype archivesPerThread =
        archiveNames.size() >= threadCount ? archiveNames.size() / threadCount : archiveNames.size();
    std::vector<std::future<void>> results;

    QVector<RS::PSARC>::Iterator psarcIter = m_psarcs.begin();
    QStringList::ConstIterator   nameIter  = archiveNames.cbegin();
    for (qsizetype i = 0; i < archiveNames.size();
         psarcIter += archivesPerThread, nameIter += archivesPerThread, i += archivesPerThread)
    {
        archivesPerThread = qMin(archiveNames.length() - i, archivesPerThread);
        results.push_back(std::async(&MusicLibrary::loadNArchives, this, psarcIter, nameIter, archivesPerThread));
    }
    for (auto& result : results)
    {
        result.get();
    }
    endResetModel();
}

Tablature* MusicLibrary::tablature()
{
    return &m_tablature;
}

void MusicLibrary::setTablature(int index, int type)
{
    m_tablature.setSNG(m_psarcs[index].sng((RS::SngType)type));
}
