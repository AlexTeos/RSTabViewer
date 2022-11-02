#ifndef NOTE_H
#define NOTE_H

#include <QVector>

#include "benddata.h"
#include "noteeffects.h"

namespace RS
{
#pragma pack(push, 1)
class Note
{
public:
    Note() : m_time(0), m_string(0xFF), m_chord(0xFFFFFFFF) { memset(m_mask, 0, sizeof(m_mask)); }

    bool isChord() const { return Effects::isChord(m_mask[0]); }
    bool isBend() const { return Effects::isBend(m_mask[0]); }
    bool isSustain() const { return Effects::isSustain(m_mask[0]); }
    bool isChordNote() const { return Effects::isChordNotes(m_mask[0]); }

    float                time() const { return m_time; }
    uint8_t              string() const { return m_string; }
    uint8_t              fret() const { return m_fret[0]; }
    uint32_t             chord() const { return m_chord; }
    uint32_t             chordNotes() const { return m_chordNotes; }
    float                sustain() const { return m_sustain; }
    const QVector<Bend>& bends() const { return m_bends; }
    QVector<Bend>&       bends() { return m_bends; }
    uint32_t             mask() const { return m_mask[0]; }

private:
    uint32_t      m_mask[2];
    uint8_t       m_u0[4];
    float         m_time;
    uint8_t       m_string;
    uint8_t       m_fret[2];
    uint8_t       m_u1;
    uint32_t      m_chord;
    uint32_t      m_chordNotes;
    uint32_t      m_phrase;
    uint32_t      m_phraseIter;
    uint16_t      m_fingerprints[2];
    uint16_t      m_u2[3];
    uint8_t       m_fingers[4];
    uint8_t       m_pickDir;
    uint8_t       m_slap;
    uint8_t       m_pluck;
    uint16_t      m_vibrato;
    float         m_sustain;
    float         m_maxbend;
    QVector<Bend> m_bends;
};
#pragma pack(pop)
}

#endif // NOTE_H
