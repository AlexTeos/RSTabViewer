#include "tablature.h"

Tablature::Tablature(QObject* parent) : QAbstractListModel(parent) {}

int Tablature::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return m_notes.size();
}

QVariant Tablature::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    bool isChord = m_notes[index.row()].m_chord != 0xFFFFFFFF;

    switch (role)
    {
        case StartTimeRole:
            return m_notes[index.row()].m_time;
        case NameRole:
            return isChord ? QVariant(m_sng.m_chords[m_notes[index.row()].m_chord].m_name) : "";
        case DurationRole:
            return index.row() == m_notes.size() - 1
                       ? QVariant(m_sng.m_metadata.m_songLength - m_notes[index.row()].m_time)
                       : QVariant(m_notes[index.row() + 1].m_time - m_notes[index.row()].m_time);
        case FretsRole:
            QList<QVariant> frets;
            if (isChord)
                for (int i = 0; i < 6; ++i)
                    frets.append(m_sng.m_chords[m_notes[index.row()].m_chord].m_frets[i]);
            else
                for (int i = 0; i < 6; ++i)
                    frets.append(i == m_notes[index.row()].m_string ? m_notes[index.row()].m_fret[0] : 0xFF);

            return QVariant(frets);
    }

    return QVariant();
}

Qt::ItemFlags Tablature::flags(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return Qt::NoItemFlags;
}

QHash<int, QByteArray> Tablature::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole]      = "name";
    names[DurationRole]  = "duration";
    names[FretsRole]     = "frets";
    names[StartTimeRole] = "startTime";

    return names;
}

bool Tablature::setSNG(const RS::SNG& sng)
{
    beginResetModel();
    m_sng    = sng;
    bool res = collectAllNotes();
    endResetModel();
    return res;
}

bool Tablature::collectAllNotes()
{
    m_notes.clear();
    QMap<float, RS::Note> notes;

    RS::Note firstNote;
    firstNote.m_time   = 0;
    firstNote.m_chord  = 0xFFFFFFFF;
    firstNote.m_string = 0xFF;
    notes.insert(0, firstNote);

    for (auto it = m_sng.m_arrangements.crbegin(); it != m_sng.m_arrangements.crend(); ++it)
    {
        for (const auto& note : (*it).m_notes)
        {
            RS::Note tmp  = note;
            float    time = note.m_time;
            if (not notes.contains(time)) notes.insert(time, tmp);
        }
    }

    m_notes.reserve(notes.size());

    for (const auto& note : notes)
    {
        m_notes.push_back(note);
    }

    return m_notes.size();
}
