#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include <QString>

class SoundBank
{
public:
    static QString getWemId(const QString& bnkFileName);
};
//https://wiki.xentax.com/index.php/Wwise_SoundBank_(*.bnk)

#endif // SOUNDBANK_H
