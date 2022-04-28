#ifndef ARRANGEMENT_H
#define ARRANGEMENT_H

#include <QIODevice>
#include <QVector>

#include "common.h"

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

struct BendData
{
    float   m_time;
    float   m_step;
    uint8_t m_unknown[4];
};

struct Note
{
    uint32_t          m_mask[2];
    uint8_t           m_u0[4];
    float             m_time;
    uint8_t           m_string;
    uint8_t           m_fret[2];
    uint8_t           m_u1;
    uint32_t          m_chord;
    uint32_t          m_chordNotes;
    uint32_t          m_phrase;
    uint32_t          m_phraseIter;
    uint16_t          m_fingerprints[2];
    uint16_t          m_u2[3];
    uint8_t           m_fingers[4];
    uint8_t           m_pickDir;
    uint8_t           m_slap;
    uint8_t           m_pluck;
    uint16_t          m_vibrato;
    float             m_sustain;
    float             m_maxbend;
    QVector<BendData> m_bends;
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

#endif // ARRANGEMENT_H
