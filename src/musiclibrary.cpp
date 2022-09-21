#include "musiclibrary.h"

MusicLibrary::MusicLibrary(const QDir& libDir, QObject* parent) : QAbstractListModel(parent)
{
    collectArchives(libDir);
}

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

void MusicLibrary::collectArchives(const QDir& dir)
{
    QStringList archiveNames = dir.entryList(QStringList() << "*.psarc", QDir::Files);
    // TODO: must be multithreaded
    for (const auto& archiveName : archiveNames)
    {
        QString archiveFullName = dir.path() + "/" + archiveName;
        QString unpackPath      = dir.path() + "/" + QFileInfo(archiveFullName).baseName();
        if (not QDir(QFileInfo(archiveFullName).completeBaseName()).exists())
            RS::PSARCArchive::unarchive(archiveFullName, unpackPath);
        m_psarcs.append(unpackPath);
    }
}

Tablature* MusicLibrary::tablature()
{
    return &m_tablature;
}

void MusicLibrary::setTablature(int index, int type)
{
    m_tablature.setSNG(m_psarcs[index].m_sngs[(RS::SngType)type]);
}
