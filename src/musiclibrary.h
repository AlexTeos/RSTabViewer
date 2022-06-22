#ifndef DICTIONARYMODEL_H
#define DICTIONARYMODEL_H

#include <QAbstractListModel>
#include <QDir>

#include "RS/psarc.h"

class MusicLibrary : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MusicLibrary(const QDir& libDir, QObject* parent = nullptr);

    // Basic functionality:
    int                            rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant                       data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags                  flags(const QModelIndex& index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    enum DictionaryRoles
    {
        SongNameRole = Qt::UserRole + 1,
        ArtistNameRole,
        AlbumNameRole,
        DurationRole,
        SongYearRole,
    };

private:
    void collectArchives(const QDir& dir);

    QVector<RS::PSARC> m_psarcs;
};

#endif // DICTIONARYMODEL_H
