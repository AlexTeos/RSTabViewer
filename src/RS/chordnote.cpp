#include "chordnote.h"

bool RS::parseChordNotes(QIODevice& input, QVector<ChordNote>& chordnotes)
{
    uint32_t chordNotesCount = ((uint32_t*)input.read(4).constData())[0];
    chordnotes.resize(chordNotesCount);

    uint32_t size = sizeof(ChordNote);
    memcpy(chordnotes.data(),
           input.read(sizeof(ChordNote) * chordNotesCount).constData(),
           sizeof(ChordNote) * chordNotesCount);

    return true;
}
