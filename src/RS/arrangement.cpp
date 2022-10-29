#include "arrangement.h"

bool RS::parseArrangements(QIODevice& input, QVector<Arrangement>& arrangements)
{
    uint32_t arrangementsCount = *((uint32_t*)input.read(4).constData());
    arrangements.resize(arrangementsCount);
    for (uint32_t i = 0; i < arrangementsCount; ++i)
    {
        // Difficulty
        arrangements[i].m_difficulty = *((uint32_t*)input.read(4).constData());
        if (arrangements[i].m_difficulty != i) return false;

        // Anchors
        int32_t anchorsCount = *((uint32_t*)input.read(4).constData());
        arrangements[i].m_anchors.resize(anchorsCount);
        memcpy(arrangements[i].m_anchors.data(),
               input.read(sizeof(Anchor) * anchorsCount).constData(),
               sizeof(Anchor) * anchorsCount);

        // AnchorExtension
        uint32_t anchorsExtentionsCount = *((uint32_t*)input.read(4).constData());
        arrangements[i].m_anchorExtensions.resize(anchorsExtentionsCount);
        memcpy(arrangements[i].m_anchorExtensions.data(),
               input.read(sizeof(AnchorExtension) * anchorsExtentionsCount).constData(),
               sizeof(AnchorExtension) * anchorsExtentionsCount);

        // FingerPrints
        for (int k = 0; k < 2; ++k)
        {
            uint32_t fingerPrintsCount = *((uint32_t*)input.read(4).constData());
            arrangements[i].m_fingerPrints[k].resize(fingerPrintsCount);
            memcpy(arrangements[i].m_fingerPrints[k].data(),
                   input.read(sizeof(FingerPrint) * fingerPrintsCount).constData(),
                   sizeof(FingerPrint) * fingerPrintsCount);
        }

        // Notes
        uint32_t notesCount = *((uint32_t*)input.read(4).constData());
        arrangements[i].m_notes.resize(notesCount);
        for (uint32_t k = 0; k < notesCount; ++k)
        {
            memcpy(&arrangements[i].m_notes[k], input.read(63).constData(), 63);

            uint32_t bendsCount = *((uint32_t*)input.read(4).constData());
            arrangements[i].m_notes[k].m_bends.resize(bendsCount);
            memcpy(arrangements[i].m_notes[k].m_bends.data(),
                   input.read(sizeof(Bend) * bendsCount).constData(),
                   sizeof(Bend) * bendsCount);
        }

        // AverageNotePerIters
        uint32_t averageNotePerItersCount = *((uint32_t*)input.read(4).constData());
        arrangements[i].m_averageNotePerIters.resize(averageNotePerItersCount);
        memcpy(arrangements[i].m_averageNotePerIters.data(),
               input.read(sizeof(float) * averageNotePerItersCount).constData(),
               sizeof(float) * averageNotePerItersCount);

        // NotesInIters
        for (int k = 0; k < 2; ++k)
        {
            uint32_t notesInItersCount = *((uint32_t*)input.read(4).constData());
            arrangements[i].m_notesInIters[k].resize(notesInItersCount);
            memcpy(arrangements[i].m_notesInIters[k].data(),
                   input.read(sizeof(uint32_t) * notesInItersCount).constData(),
                   sizeof(uint32_t) * notesInItersCount);
        }
    }

    return true;
}
