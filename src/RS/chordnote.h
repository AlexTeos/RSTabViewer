#ifndef CHORDNOTE_H
#define CHORDNOTE_H

#include <QIODevice>
#include <QVector>

#include "benddata.h"
#include "noteeffects.h"

namespace RS
{
#pragma pack(push, 1)
struct ChordNote
{
public:
    bool isChord(int index) const { return Effects::isChord(m_noteMask[index]); }
    bool isBend(int index) const { return Effects::isBend(m_noteMask[index]); }
    bool isSustain(int index) const { return Effects::isSustain(m_noteMask[index]); }
    bool isChordNotes(int index) const { return Effects::isChordNotes(m_noteMask[index]); }

    uint32_t noteMask(int index) const { return m_noteMask[index]; }

private:
    uint32_t m_noteMask[6];
    Bend32   m_bends[6];
    uint8_t  m_startFretId[6];
    uint8_t  m_endFretId[6];
    uint16_t m_name[6];
};
#pragma pack(pop)

bool parseChordNotes(QIODevice& input, QVector<ChordNote>& chords);
}
#endif // CHORDNOTE_H
