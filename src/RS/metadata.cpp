#include "metadata.h"

bool RS::parseMetadata(QIODevice& input, Metadata& metadata)
{
    // Maxscore
    metadata.m_maxScore = *((double*)input.read(8).constData());
    // Max_note_and_chords
    metadata.m_maxNoteAndChords = *((double*)input.read(8).constData());
    // U0
    metadata.m_u0 = *((double*)input.read(8).constData());
    // Points per note
    metadata.m_pointsPerNote = *((double*)input.read(8).constData());
    // First beat length
    metadata.m_firstBeatLength = *((float*)input.read(4).constData());
    // Start Time
    metadata.m_startTime = *((float*)input.read(4).constData());
    // Capo fret
    metadata.m_capoFret = *input.read(1).constData();
    // Last conversion date
    memcpy(metadata.m_lastConversionDate,
           input.read(sizeof(metadata.m_lastConversionDate)).constData(),
           sizeof(metadata.m_lastConversionDate));
    // Part
    metadata.m_part = *((uint16_t*)input.read(2).constData());
    // Song length
    metadata.m_songLength = *((float*)input.read(4).constData());
    // Tuning
    int32_t tuningCount = *((uint32_t*)input.read(4).constData());
    metadata.m_tuning.resize(tuningCount);
    memcpy(metadata.m_tuning.data(),
           input.read(sizeof(uint16_t) * tuningCount).constData(),
           sizeof(uint16_t) * tuningCount);
    // U1
    memcpy(&metadata.m_u1, input.read(sizeof(float) * 2).constData(), sizeof(float) * 2);
    // Max difficulty
    metadata.m_maxDifficulty = *((uint32_t*)input.read(4).constData());

    return true;
}
