#include "tst_psarc.h"

#include <ww2ogg.h>

#include "../src/RS/3rdparty/revorb/revorb.h"

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
    foreach(auto archiveName, m_archiveNames) { QVERIFY(RS::PSARCArchive::unarchive(archiveName)); }
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

void TestPSARC::testWW2OGG()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QStringList audioNames =
            QDir(archiveName + "\\audio\\windows").entryList(QStringList() << "*.wem", QDir::Files);
        QVERIFY(m_archiveNames.size());

        for (const QString& audio : audioNames)
        {
            QString filePath    = archiveName + "\\audio\\windows\\" + audio;
            QString outFilePath = filePath;
            outFilePath.replace(QStringLiteral(".wem"), QStringLiteral(".ogg"), Qt::CaseInsensitive);
            QVERIFY(ww2ogg(filePath.toStdString(), outFilePath.toStdString()));
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
        QVERIFY(m_archiveNames.size());

        for (const QString& audio : audioNames)
        {
            QVERIFY(revorb((archiveName + "\\audio\\windows\\" + audio).toStdString()));
        }
    }
}
