#ifndef SNG_H
#define SNG_H

#include <QFile>
#include <QString>

#include "arrangement.h"

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
    QFile                m_sngFile;
    QFile                m_sngDecryptedFile;
    QVector<Arrangement> m_arrangements;
};

#endif // SNG_H
