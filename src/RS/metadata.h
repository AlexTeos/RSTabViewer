#ifndef METADATA_H
#define METADATA_H

#include <QIODevice>

#include "common.h"

namespace RS
{
struct Metadata
{
    double            m_maxScore;
    double            m_maxNoteAndChords;
    double            m_u0;
    double            m_pointsPerNote;
    float             m_firstBeatLength;
    float             m_startTime;
    uint8_t           m_capoFret;
    char              m_lastConversionDate[32];
    uint16_t          m_part;
    float             m_songLength;
    QVector<uint16_t> m_tuning;
    float             m_u1[2];
    uint32_t          m_maxDifficulty;
};

bool parseMetadata(QIODevice& input, Metadata& metadata);
}

#endif // METADATA_H
