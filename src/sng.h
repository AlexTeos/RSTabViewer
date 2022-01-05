#ifndef SNG_H
#define SNG_H

#include <QFile>
#include <QString>

class SNG
{
public:
    SNG(){};
    SNG(const QString& sngFileName);
    bool decrypt();

    void setSngFile(const QString& newSngFileName);

private:
    QFile m_sngFile;
};

#endif // SNG_H
