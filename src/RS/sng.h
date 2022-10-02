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
        PalmMute = 0x40,
        Sustain  = 0x2000
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
