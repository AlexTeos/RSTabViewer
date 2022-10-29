#ifndef SNG_H
#define SNG_H

#include <QFile>
#include <QString>

#include "arrangement.h"
#include "chord.h"
#include "chordnote.h"
#include "metadata.h"

namespace RS
{
class SNG
{
public:
    enum Type
    {
        Bass = 0,
        Lead,
        Rhythm,
        Vocals,
        Combo,
        Showlights
    };

    enum MaskFlags
    {
        ChordFlag          = 0x00000002,
        FretHandMuteFlag   = 0x00000008,
        TremoloFlag        = 0x00000010,
        HarmonicFlag       = 0x00000020,
        PalmMuteFlag       = 0x00000040,
        HammerOnFlag       = 0x00000200,
        PullOffFlag        = 0x00000400,
        SlideFlag          = 0x00000800,
        BendFlag           = 0x00001000,
        SustainFlag        = 0x00002000,
        PinchHarmonicFlag  = 0x00008000,
        VibratoFlag        = 0x00010000,
        MuteFlag           = 0x00020000,
        UnpitchedSlideFlag = 0x00400000,
        SingleFlag         = 0x00800000,
        AccentFlag         = 0x04000000,
        ParentFlag         = 0x08000000,
        ChildFlag          = 0x10000000
    };

    SNG(Type type, const QString& file);

    const QVector<Arrangement>& arrangements() const;
    const QVector<Chord>&       chords() const;
    const Metadata&             metadata() const;
    bool                        initialize() const;

    Type type() const;

private:
    bool decrypt(const QString& sngFileName) const;
    bool parse(const QString& decryptedSngFileName) const;
    bool dummyRead(QIODevice& input, const qint64& structureSize, const qint64& additionalSize, uint32_t& count) const;

    Type                         m_type;
    mutable QVector<Arrangement> m_arrangements;
    mutable Metadata             m_metadata;
    mutable QVector<Chord>       m_chords;
    mutable QVector<ChordNote>   m_chordNotes;
    mutable bool                 m_initialized = false;
    QString                      m_file;
};
}

#endif // SNG_H
