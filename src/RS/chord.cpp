#include "chord.h"

bool RS::parseChords(QIODevice& input, QVector<Chord>& chords)
{
    uint32_t chordsCount = ((uint32_t*)input.read(4).constData())[0];
    chords.resize(chordsCount);

    memcpy(chords.data(), input.read(sizeof(Chord) * chordsCount).constData(), sizeof(Chord) * chordsCount);

    return true;
}
