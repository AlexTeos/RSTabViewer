#include "tst_psarc.h"

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
        RS::PSARC psarc;
        psarc.setPsarcFile(archiveName);
        QVERIFY(psarc.unarchive());
    }
}

void TestPSARC::testDecrypt()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QStringList sngNames =
            QDir(archiveName + "\\songs\\bin\\generic").entryList(QStringList() << "*_lead.sng", QDir::Files);
        QVERIFY(m_archiveNames.size());

        QString sngName = sngNames[0];
        RS::SNG sng;
        QVERIFY(sng.decrypt(archiveName + "\\songs\\bin\\generic\\" + sngName));
    }
}

void TestPSARC::testParse()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QStringList sngNames =
            QDir(archiveName + "\\songs\\bin\\generic").entryList(QStringList() << "*_uncompressed.sng", QDir::Files);
        QVERIFY(m_archiveNames.size());

        QString sngName = sngNames[0];
        RS::SNG sng;
        QVERIFY(sng.parse(archiveName + "\\songs\\bin\\generic\\" + sngName));
        m_parsedSng.push_back(sng);
    }
}
