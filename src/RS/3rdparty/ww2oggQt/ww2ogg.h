#ifndef WW2OGG_H
#define WW2OGG_H

#include <QDir>
#include <QTemporaryDir>

#include "ww2ogg/src/wwriff.h"

bool ww2ogg(const string& filename, const string& out_filename)
{
    // TODO: don't copy everytime
    Q_INIT_RESOURCE(codebooks);
    QTemporaryDir temporaryDir;
    string        temporaryCodebookFileName = temporaryDir.path().toStdString() + "/packed_codebooks_aoTuV_603.bin";
    QFile::copy(":/ww2ogg/packed_codebooks_aoTuV_603.bin", QString::fromLatin1(temporaryCodebookFileName.c_str()));

    try
    {
        Wwise_RIFF_Vorbis ww(filename, temporaryCodebookFileName, false, false, kNoForcePacketFormat);

        ofstream of(out_filename.c_str(), ios::binary);
        if (!of) return false;

        ww.generate_ogg(of);
    }
    catch (const File_open_error& fe)
    {
        return false;
    }
    catch (const Parse_error& pe)
    {
        return false;
    }

    return true;
}

#endif // WW2OGG_H
