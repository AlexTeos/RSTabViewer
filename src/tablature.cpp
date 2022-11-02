#include "tablature.h"

Tablature::Tablature(QObject* parent) : QAbstractListModel(parent), m_sng(RS::SNG::Type::Bass, "") {}

int Tablature::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return m_notes.size();
}

QVariant Tablature::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();
    QList<QVariant> returnList;
    switch (role)
    {
        case StartTimeRole:
            return m_notes[index.row()].time();
        case DurationRole:
            return index.row() == m_notes.size() - 1
                       ? QVariant(m_sng.metadata().m_songLength - m_notes[index.row()].time())
                       : QVariant(m_notes[index.row() + 1].time() - m_notes[index.row()].time());
        case FretsRole:
        {
            QList<QVariant> returnList;
            if (m_notes[index.row()].isChord())
                for (int i = 0; i < 6; ++i)
                    returnList.append(m_sng.chords()[m_notes[index.row()].chord()].m_frets[i]);
            else
                for (int i = 0; i < 6; ++i)
                    returnList.append(i == m_notes[index.row()].string() ? m_notes[index.row()].fret() : 0xFF);
            return returnList;
        }
        case SustainRole:
            return (m_notes[index.row()].isSustain()) ? m_notes[index.row()].sustain() : 0;
        case BendStepsRole:
            if (not(m_notes[index.row()].isBend())) return QList<QVariant>();
            returnList.reserve(m_notes[index.row()].bends().size());
            for (int i = 0; i < m_notes[index.row()].bends().size(); ++i)
                returnList.push_back(m_notes[index.row()].bends()[i].m_step);
            return returnList;
        case BendStartsRole:
            if (not(m_notes[index.row()].isBend())) return QList<QVariant>();
            returnList.reserve(m_notes[index.row()].bends().size());
            for (int i = 0; i < m_notes[index.row()].bends().size(); ++i)
                returnList.push_back(m_notes[index.row()].bends()[i].m_time - m_notes[index.row()].time());
            return returnList;
        case ChordNoteRole:
            return m_notes[index.row()].isChordNote();
        case MaskRole:
            if (m_notes[index.row()].isChordNote())
            {
                returnList.reserve(6);
                for (int i = 0; i < 6; ++i)
                    returnList.push_back(m_sng.chordNotes()[m_notes[index.row()].chordNotes()].noteMask(i));
                return returnList;
            }
            return m_notes[index.row()].mask();
        case NextFretsRole:
            if (index.row() == m_notes.size() - 1) return QList<QVariant>();
            QList<QVariant> frets;
            if (m_notes[index.row() + 1].isChord())
                for (int i = 0; i < 6; ++i)
                    frets.append(m_sng.chords()[m_notes[index.row() + 1].chord()].m_frets[i]);
            else
                for (int i = 0; i < 6; ++i)
                    frets.append(i == m_notes[index.row() + 1].string() ? m_notes[index.row() + 1].fret() : 0xFF);
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
    names[NameRole]       = "name";
    names[DurationRole]   = "duration";
    names[FretsRole]      = "frets";
    names[StartTimeRole]  = "startTime";
    names[SustainRole]    = "sustain";
    names[BendStepsRole]  = "bendSteps";
    names[BendStartsRole] = "bendStarts";
    names[NextFretsRole]  = "nextFrets";
    names[ChordNoteRole]  = "chordNote";
    names[MaskRole]       = "mask";

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
    notes.insert(0, firstNote);

    for (auto it = m_sng.arrangements().crbegin(); it != m_sng.arrangements().crend(); ++it)
    {
        for (const auto& note : (*it).m_notes)
        {
            RS::Note tmp  = note;
            float    time = note.time();
            if (not notes.contains(time)) notes.insert(time, tmp);
        }
    }

    m_notes.reserve(notes.size());

    for (const auto& note : notes)
        m_notes.push_back(note);

    return m_notes.size();
}
