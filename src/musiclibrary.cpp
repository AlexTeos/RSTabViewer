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
    names[SongNameRole]   = "songName";
    names[ArtistNameRole] = "artistName";
    names[AlbumNameRole]  = "albumName";
    names[DurationRole]   = "duration";
    names[SongYearRole]   = "songYear";

    return names;
}

void MusicLibrary::collectArchives(const QDir& dir)
{
    QStringList archiveNames = dir.entryList(QStringList() << "*.psarc", QDir::Files);
    for (const auto& archiveName : archiveNames)
    {
        if (not QDir(QFileInfo(archiveName).completeBaseName()).exists())
            RS::PSARCArchive::unarchive(archiveName, QFileInfo(archiveName).completeBaseName());
        m_psarcs.append(QFileInfo(archiveName).completeBaseName());
    }
}
