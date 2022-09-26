#include "tst_psarc.h"

#include <ww2ogg.h>

#include "../src/RS/3rdparty/revorb/revorb.h"
#include "../src/RS/soundbank.h"

void TestPSARC::initTestCase()
{
    m_archiveNames = QDir().entryList(QStringList() << "*.psarc", QDir::Files);
    QVERIFY(m_archiveNames.size());
}

void TestPSARC::cleanupTestCase()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QDir dir(archiveName);
        dir.removeRecursively();
    }
}

void TestPSARC::testUnarchive()
{
    foreach(auto archiveName, m_archiveNames)
    {
        QVERIFY2(RS::PSARCArchive::unarchive(archiveName, QFileInfo(archiveName).completeBaseName()),
                 archiveName.toLatin1());
    }
}

void TestPSARC::testDecrypt()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QStringList sngNames =
            QDir(archiveName + "\\songs\\bin\\generic").entryList(QStringList() << "*.sng", QDir::Files);
        QVERIFY2(sngNames.size(), archiveName.toLatin1());

        for (const auto& sngName : sngNames)
        {
            RS::SNG sng;
            QVERIFY2(sng.decrypt(archiveName + "\\songs\\bin\\generic\\" + sngName), archiveName.toLatin1());
        }
    }
}

void TestPSARC::testParse()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QStringList sngNames =
            QDir(archiveName + "\\songs\\bin\\generic").entryList(QStringList() << "*.snguc", QDir::Files);
        QVERIFY2(sngNames.size(), archiveName.toLatin1());

        for (const auto& sngName : sngNames)
        {
            RS::SNG sng;
            sng.setDecryptedFile(archiveName + "\\songs\\bin\\generic\\" + sngName);
            if (sngName.contains("vocals")) continue;
            QVERIFY2(sng.arrangements().size(), archiveName.toLatin1());
        }
    }
}

void TestPSARC::testSoundBank()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QStringList bnkNames = QDir(archiveName + "\\audio\\windows").entryList(QStringList() << "*.bnk", QDir::Files);
        QVERIFY2(bnkNames.size(), archiveName.toLatin1());

        for (const QString& bnk : bnkNames)
        {
            QString filePath = archiveName + "\\audio\\windows\\" + bnk;
            QVERIFY2(SoundBank::getWemId(filePath) != 0, archiveName.toLatin1());
        }
    }
}

void TestPSARC::testWW2OGG()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QStringList audioNames =
            QDir(archiveName + "\\audio\\windows").entryList(QStringList() << "*.wem", QDir::Files);
        QVERIFY2(audioNames.size(), archiveName.toLatin1());

        for (const QString& audio : audioNames)
        {
            QString filePath    = archiveName + "\\audio\\windows\\" + audio;
            QString outFilePath = filePath;
            outFilePath.replace(QStringLiteral(".wem"), QStringLiteral(".ogg"), Qt::CaseInsensitive);
            QVERIFY2(ww2ogg(filePath.toStdString(), outFilePath.toStdString()), archiveName.toLatin1());
        }
    }
}

void TestPSARC::testRevorb()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QStringList audioNames =
            QDir(archiveName + "\\audio\\windows").entryList(QStringList() << "*.ogg", QDir::Files);
        QVERIFY2(audioNames.size(), archiveName.toLatin1());

        for (const QString& audio : audioNames)
        {
            QVERIFY2(revorb((archiveName + "\\audio\\windows\\" + audio).toStdString()), archiveName.toLatin1());
        }
    }
}
