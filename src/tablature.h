#ifndef TABLATUREYMODEL_H
#define TABLATUREYMODEL_H

#include <QAbstractListModel>

#include "RS/sng.h"

class Tablature : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit Tablature(QObject* parent = nullptr);

    // Basic functionality:
    int                            rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant                       data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags                  flags(const QModelIndex& index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    enum DictionaryRoles
    {
        NameRole = Qt::UserRole + 1,
        DurationRole,
        FretsRole,
        StartTimeRole,
        SustainRole,
        MuteRole,
        ParentRole,
        SlideRole,
        NextFretsRole,
        ChildRole
    };

    bool setSNG(const RS::SNG& sng);

private:
    bool collectAllNotes();

    RS::SNG           m_sng;
    QVector<RS::Note> m_notes;
};

#endif // TABLATUREYMODEL_H
