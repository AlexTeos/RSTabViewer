#include "arrangement.h"

QVector<Arrangement> parseArrangements(QIODevice& input)
{
    QVector<Arrangement> arrangements;
    uint32_t             arrangementsCount = READ_LE_UINT32((uint8_t*)input.read(4).constData());
    arrangements.resize(arrangementsCount);
    for (uint32_t i = 0; i < arrangementsCount; ++i)
    {
        // Difficulty
        arrangements[i].difficulty = READ_LE_UINT32((uint8_t*)input.read(4).constData());

        // Anchors
        int32_t anchorsCount = READ_LE_UINT32((uint8_t*)input.read(4).constData());
        arrangements[i].anchors.resize(anchorsCount);
        memcpy(arrangements[i].anchors.data(),
               input.read(sizeof(Anchor) * anchorsCount).constData(),
               sizeof(Anchor) * anchorsCount);

        // AnchorExtension
        uint32_t anchorsExtentionsCount = READ_LE_UINT32((uint8_t*)input.read(4).constData());
        arrangements[i].anchorExtensions.resize(anchorsExtentionsCount);
        memcpy(arrangements[i].anchorExtensions.data(),
               input.read(sizeof(AnchorExtension) * anchorsExtentionsCount).constData(),
               sizeof(AnchorExtension) * anchorsExtentionsCount);

        // FingerPrints
        for (int k = 0; k < 2; ++k)
        {
            uint32_t fingerPrintsCount = READ_LE_UINT32((uint8_t*)input.read(4).constData());
            arrangements[i].fingerPrints[k].resize(fingerPrintsCount);
            memcpy(arrangements[i].fingerPrints[k].data(),
                   input.read(sizeof(FingerPrint) * fingerPrintsCount).constData(),
                   sizeof(FingerPrint) * fingerPrintsCount);
        }

        // Notes
        uint32_t notesCount = READ_LE_UINT32((uint8_t*)input.read(4).constData());
        arrangements[i].notes.resize(notesCount);
        for (uint32_t k = 0; k < notesCount; ++k)
        {
            memcpy(&arrangements[i].notes[k], input.read(63).constData(), 63);

            uint32_t bendsCount = READ_LE_UINT32((uint8_t*)input.read(4).constData());
            arrangements[i].notes[k].bends.resize(bendsCount);
            memcpy(arrangements[i].notes[k].bends.data(),
                   input.read(sizeof(float) * bendsCount).constData(),
                   sizeof(float) * bendsCount);
        }

        // AverageNotePerIters
        uint32_t averageNotePerItersCount = READ_LE_UINT32((uint8_t*)input.read(4).constData());
        arrangements[i].averageNotePerIters.resize(averageNotePerItersCount);
        memcpy(arrangements[i].averageNotePerIters.data(),
               input.read(sizeof(float) * averageNotePerItersCount).constData(),
               sizeof(float) * averageNotePerItersCount);

        // NotesInIters
        for (int k = 0; k < 2; ++k)
        {
            uint32_t notesInItersCount = READ_LE_UINT32((uint8_t*)input.read(4).constData());
            arrangements[i].notesInIters[k].resize(notesInItersCount);
            memcpy(arrangements[i].notesInIters[k].data(),
                   input.read(sizeof(uint32_t) * notesInItersCount).constData(),
                   sizeof(uint32_t) * notesInItersCount);
        }
    }

    return arrangements;
}
