#ifndef CHORDNOTE_H
#define CHORDNOTE_H

#include <QIODevice>
#include <QVector>

#include "benddata.h"

namespace RS
{
#pragma pack(push, 1)
struct ChordNote
{
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
