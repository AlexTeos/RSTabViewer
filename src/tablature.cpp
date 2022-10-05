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

    bool isChord = m_notes[index.row()].m_chord != 0xFFFFFFFF;

    switch (role)
    {
        case StartTimeRole:
            return m_notes[index.row()].m_time;
        case NameRole:
            return isChord ? QVariant(m_sng.chords()[m_notes[index.row()].m_chord].m_name) : "";
        case DurationRole:
            return index.row() == m_notes.size() - 1
                       ? QVariant(m_sng.metadata().m_songLength - m_notes[index.row()].m_time)
                       : QVariant(m_notes[index.row() + 1].m_time - m_notes[index.row()].m_time);
        case FretsRole:
        {
            QList<QVariant> frets;
            if (isChord)
                for (int i = 0; i < 6; ++i)
                    frets.append(m_sng.chords()[m_notes[index.row()].m_chord].m_frets[i]);
            else
                for (int i = 0; i < 6; ++i)
                    frets.append(i == m_notes[index.row()].m_string ? m_notes[index.row()].m_fret[0] : 0xFF);
            return QVariant(frets);
        }
        case SustainRole:
            if (m_notes[index.row()].m_mask[0] & RS::SNG::MaskFlags::Slide)
            {
                if (index.row() == m_notes.size() - 1) return m_notes[index.row()].m_sustain;
                return m_notes[index.row()].m_sustain + m_notes[index.row() + 1].m_sustain;
            }
            else
            {
                return (m_notes[index.row()].m_mask[0] & RS::SNG::MaskFlags::Sustain) ? m_notes[index.row()].m_sustain
                                                                                      : 0;
            }
        case MuteRole:
            return m_notes[index.row()].m_mask[0] & (RS::SNG::MaskFlags::PalmMute | RS::SNG::MaskFlags::Mute);
        case SlideRole:
            return m_notes[index.row()].m_mask[0] & RS::SNG::MaskFlags::Slide;
        case ParentRole:
            return m_notes[index.row()].m_mask[0] & RS::SNG::MaskFlags::Parent;
        case ChildRole:
            // TODO: check (see Reptilia)
            if (index.row() != 0 and m_notes[index.row() - 1].m_mask[0] & RS::SNG::MaskFlags::Slide) return true;
            return m_notes[index.row()].m_mask[0] & RS::SNG::MaskFlags::Child;
        case NextFretsRole:
            // TODO: return only 1 value
            if (index.row() == m_notes.size() - 1) return QList<QVariant>();
            QList<QVariant> frets;
            if (isChord)
                for (int i = 0; i < 6; ++i)
                    frets.append(m_sng.chords()[m_notes[index.row() + 1].m_chord].m_frets[i]);
            else
                for (int i = 0; i < 6; ++i)
                    frets.append(i == m_notes[index.row() + 1].m_string ? m_notes[index.row() + 1].m_fret[0] : 0xFF);
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
    names[SustainRole]   = "sustain";
    names[MuteRole]      = "mute";
    names[ParentRole]    = "parentNote";
    names[SlideRole]     = "slide";
    names[NextFretsRole] = "nextFrets";
    names[ChildRole]     = "childNote";

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

    for (auto it = m_sng.arrangements().crbegin(); it != m_sng.arrangements().crend(); ++it)
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
