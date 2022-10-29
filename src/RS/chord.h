#ifndef CHORD_H
#define CHORD_H

#include <QIODevice>
#include <QVector>

namespace RS
{
#pragma pack(push, 1)
struct Chord
{
    uint32_t m_mask;
    uint8_t  m_frets[6];
    uint8_t  m_fingers[6];
    uint32_t m_notes[6];
    char     m_name[32];
};
#pragma pack(pop)

bool parseChords(QIODevice& input, QVector<Chord>& chords);
}
#endif // CHORD_H
