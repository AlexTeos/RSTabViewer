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
    bool decrypt(const QString& sngFileName);

    void                        setDecryptedFile(const QString& newDecryptedFile);
    const QVector<Arrangement>& arrangements() const;
    const QVector<Chord>&       chords() const;
    const Metadata&             metadata() const;

private:
    bool parse(const QString& decryptedSngFileName) const;
    bool dummyRead(QIODevice& input, const qint64& structureSize, const qint64& additionalSize, uint32_t& count) const;

    mutable QVector<Arrangement> m_arrangements;
    mutable Metadata             m_metadata;
    mutable QVector<Chord>       m_chords;
    mutable bool                 m_parsed = false;
    QString                      m_decryptedFile;
};
}

#endif // SNG_H
