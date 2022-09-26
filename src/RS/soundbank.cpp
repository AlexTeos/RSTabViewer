#include "soundbank.h"

#include <QFile>

struct Section
{
    char     identifier[4];
    uint32_t length;
};

struct DIDXSection
{
    uint32_t id;
    uint32_t offset;
    uint32_t length;
};

QString SoundBank::getWemId(const QString& bnkFileName)
{
    QFile bnk(bnkFileName);

    if (bnk.open(QIODevice::ReadOnly))
    {
        Section section;
        while (bnk.read(reinterpret_cast<char*>(&section), sizeof(Section)))
        {
            if (strncmp(section.identifier, "DIDX", 4) == 0)
            {
                DIDXSection didxSection;
                bnk.read(reinterpret_cast<char*>(&didxSection), sizeof(DIDXSection));

                return QString::number(didxSection.id);
            }
            else
            {
                bnk.read(section.length);
            }
        }
    }
    return "";
}
