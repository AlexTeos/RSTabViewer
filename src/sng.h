#ifndef SNG_H
#define SNG_H

#include <QFile>
#include <QString>

#include "arrangement.h"
#include "chord.h"

class SNG
{
public:
    SNG(){};
    SNG(const QString& sngFileName);
    bool decrypt();
    bool parse();

    void setSngFile(const QString& newSngFileName);
    void setSngDecryptedFile(const QString& newSngDecryptedFileName);

private:
    bool dummyRead(QIODevice& input, const qint64& structureSize, const qint64& additionalSize, uint32_t& count);

    QFile                m_sngFile;
    QFile                m_sngDecryptedFile;
    QVector<Chord>       m_chords;
    QVector<Arrangement> m_arrangements;
};

#endif // SNG_H
