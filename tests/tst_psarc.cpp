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
        QDir dir(archiveName.first(archiveName.size() - 6));
        dir.removeRecursively();
    }
}

void TestPSARC::testUnarchive()
{
    foreach(auto archiveName, m_archiveNames)
    {
        PSARC psarc;
        psarc.setPsarcFile(archiveName);
        QVERIFY(psarc.unarchive());
    }
}

void TestPSARC::testDecrypt()
{
    foreach(QString archiveName, m_archiveNames)
    {
        QStringList sngNames = QDir(archiveName.first(archiveName.size() - 6) + "\\songs\\bin\\generic")
                                   .entryList(QStringList() << "*_lead.sng", QDir::Files);
        QVERIFY(m_archiveNames.size());

        QString sngName = sngNames[0];
        SNG     sng;
        QVERIFY(sng.decrypt(archiveName.first(archiveName.size() - 6) + "\\songs\\bin\\generic\\" + sngName));
    }
}

void TestPSARC::testParse()
{
    foreach(QString archiveName, m_archiveNames)
    {
        QStringList sngNames = QDir(archiveName.first(archiveName.size() - 6) + "\\songs\\bin\\generic")
                                   .entryList(QStringList() << "*_uncompressed.sng", QDir::Files);
        QVERIFY(m_archiveNames.size());

        QString sngName = sngNames[0];
        SNG     sng;
        QVERIFY(sng.parse(archiveName.first(archiveName.size() - 6) + "\\songs\\bin\\generic\\" + sngName));
        m_parsedSng.push_back(sng);
    }
}

void TestPSARC::testGetTablature()
{
    for (const auto& sng : m_parsedSng)
    {
        QVERIFY(sng.getTablature().size());
    }
}
