#ifndef SNG_H
#define SNG_H

#include <QFile>
#include <QString>

#include "arrangement.h"
#include "chord.h"
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
        Tremolo        = 0x00000010,
        Harmonic       = 0x00000020,
        PalmMute       = 0x00000040,
        HammerOn       = 0x00000200,
        PullOff        = 0x00000400,
        Slide          = 0x00000800,
        Sustain        = 0x00002000,
        Vibrato        = 0x00010000,
        Mute           = 0x00020000,
        UnpitchedSlide = 0x00400000,
        Single         = 0x00800000,
        Parent         = 0x08000000,
        Child          = 0x10000000
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
    mutable bool                 m_initialized = false;
    QString                      m_file;
};
}

#endif // SNG_H
