#ifndef ARRANGEMENT_H
#define ARRANGEMENT_H

#include <QIODevice>
#include <QVector>

#include "note.h"

namespace RS
{
#pragma pack(push, 1)
struct AnchorExtension
{
    uint32_t m_beatTime;
    uint8_t  m_fret;
    uint8_t  m_unknown[7];
};

struct Anchor
{
    float    m_startBeat;
    float    m_endBeat;
    uint8_t  m_unknown[8];
    uint32_t m_fret;
    uint32_t m_width;
    uint32_t m_phraseIter;
};

struct FingerPrint
{
    uint32_t m_chord;
    float    m_start;
    float    m_end;
    uint8_t  m_unknown[8];
};

struct Arrangement
{
    uint32_t                 m_difficulty;
    QVector<Anchor>          m_anchors;
    QVector<AnchorExtension> m_anchorExtensions;
    QVector<FingerPrint>     m_fingerPrints[2];
    QVector<Note>            m_notes;
    QVector<float>           m_averageNotePerIters;
    QVector<uint32_t>        m_notesInIters[2];
};
#pragma pack(pop)

bool parseArrangements(QIODevice& input, QVector<Arrangement>& arrangements);
}
#endif // ARRANGEMENT_H
