#ifndef ARRANGEMENT_H
#define ARRANGEMENT_H

#include <QIODevice>
#include <QVector>

#include "common.h"

#pragma pack(push, 1)

struct AnchorExtension
{
    uint32_t beatTime;
    uint8_t  fret;
    uint8_t  unknown[7];
};

struct Anchor
{
    float    startBeat;
    float    endBeat;
    uint8_t  unknown[8];
    uint32_t fret;
    uint32_t width;
    uint32_t phraseIter;
};

struct FingerPrint
{
    uint32_t chord;
    float    start;
    float    end;
    uint8_t  unknown[8];
};

struct BendData
{
    float   time;
    float   step;
    uint8_t unknown[4];
};

struct Note
{
    uint32_t          mask[2];
    uint8_t           u0[4];
    float             time;
    uint8_t           string;
    uint8_t           fret[2];
    uint8_t           u1;
    uint32_t          chord;
    uint32_t          chordNotes;
    uint32_t          phrase;
    uint32_t          phraseIter;
    uint16_t          fingerprints[2];
    uint16_t          u2[3];
    uint8_t           fingers[4];
    uint8_t           pickDir;
    uint8_t           slap;
    uint8_t           pluck;
    uint16_t          vibrato;
    float             sustain;
    float             maxbend;
    QVector<BendData> bends;
};

struct Arrangement
{
    uint32_t                 difficulty;
    QVector<Anchor>          anchors;
    QVector<AnchorExtension> anchorExtensions;
    QVector<FingerPrint>     fingerPrints[2];
    QVector<Note>            notes;
    QVector<float>           averageNotePerIters;
    QVector<uint32_t>        notesInIters[2];
};

#pragma pack(pop)

QVector<Arrangement> parseArrangements(QIODevice& input);

#endif // ARRANGEMENT_H
