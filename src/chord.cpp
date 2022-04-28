#include "chord.h"

bool parseChords(QIODevice& input, QVector<Chord>& chords)
{
    uint32_t chordsCount = READ_LE_UINT32((uint8_t*)input.read(4).constData());
    chords.resize(chordsCount);

    memcpy(chords.data(), input.read(sizeof(Chord) * chordsCount).constData(), sizeof(Chord) * chordsCount);

    return true;
}
