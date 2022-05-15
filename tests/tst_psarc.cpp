#include "tst_psarc.h"

void TestPSARC::initTestCase()
{
    archiveNames = QDir().entryList(QStringList() << "*.psarc", QDir::Files);
    QVERIFY(archiveNames.size());
}

void TestPSARC::cleanupTestCase()
{
    foreach(QString archiveName, archiveNames)
    {
        QDir dir(archiveName.first(archiveName.size() - 6));
        dir.removeRecursively();
    }
}

void TestPSARC::testUnarchive()
{
    foreach(auto archiveName, archiveNames)
    {
        PSARC psarc;
        psarc.setPsarcFile(archiveName);
        QVERIFY(psarc.unarchive());
    }
}

void TestPSARC::testDecrypt()
{
    foreach(QString archiveName, archiveNames)
    {
        QStringList sngNames = QDir(archiveName.first(archiveName.size() - 6) + "\\songs\\bin\\generic")
                                   .entryList(QStringList() << "*_lead.sng", QDir::Files);
        QVERIFY(archiveNames.size());

        QString sngName = sngNames[0];
        SNG     sng;
        sng.setSngFile(archiveName.first(archiveName.size() - 6) + "\\songs\\bin\\generic\\" + sngName);
        QVERIFY(sng.decrypt());
    }
}

void TestPSARC::testParse()
{
    foreach(QString archiveName, archiveNames)
    {
        QStringList sngNames = QDir(archiveName.first(archiveName.size() - 6) + "\\songs\\bin\\generic")
                                   .entryList(QStringList() << "*_uncompressed.sng", QDir::Files);
        QVERIFY(archiveNames.size());

        QString sngName = sngNames[0];
        SNG     sng;
        sng.setSngDecryptedFile(archiveName.first(archiveName.size() - 6) + "\\songs\\bin\\generic\\" + sngName);
        QVERIFY(sng.parse());
    }
}
