#ifndef SNG_H
#define SNG_H

#include <QFile>
#include <QString>

#include "arrangement.h"
#include "chord.h"

struct Beat
{
    uint8_t m_frets[6];
};

namespace RS
{
class SNG
{
public:
    bool decrypt(const QString& sngFileName);
    bool parse(const QString& decryptedSngFileName);

    QMap<float, Note> getNotes() const;
    QMap<float, Beat> getTablature() const;

private:
    bool dummyRead(QIODevice& input, const qint64& structureSize, const qint64& additionalSize, uint32_t& count);

    QVector<Chord>       m_chords;
    QVector<Arrangement> m_arrangements;
};
}

#endif // SNG_H
