#ifndef SNG_H
#define SNG_H

#include <QFile>
#include <QString>

#include "arrangement.h"
#include "chord.h"
#include "metadata.h"

namespace RS
{
struct SNG
{
    bool decrypt(const QString& sngFileName);
    bool parse(const QString& decryptedSngFileName);
    bool dummyRead(QIODevice& input, const qint64& structureSize, const qint64& additionalSize, uint32_t& count);

    Metadata             m_metadata;
    QVector<Chord>       m_chords;
    QVector<Arrangement> m_arrangements;
};
}

#endif // SNG_H
