#include "tst_psarc.h"

#include <QDirIterator>

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

void TestPSARC::testSng()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QDirIterator it(archiveName, QStringList() << "*.sng", QDir::Files, QDirIterator::Subdirectories);
        QVERIFY(it.hasNext());
        while (it.hasNext())
        {
            RS::SNG sng(RS::SNG::Type::Bass, it.next());
            if (it.filePath().contains("vocals")) continue;
            QVERIFY(sng.arrangements().size());
        }
    }
}

void TestPSARC::testSoundBank()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QDirIterator it(archiveName, QStringList() << "*.bnk", QDir::Files, QDirIterator::Subdirectories);
        QVERIFY(it.hasNext());
        while (it.hasNext())
        {
            QString filePath = it.next();
            QVERIFY2(SoundBank::getWemId(filePath) != 0, archiveName.toLatin1());
        }
    }
}

void TestPSARC::testWW2OGG()
{
    foreach(QString archiveName, m_archiveNames)
    {
        archiveName.truncate(archiveName.size() - 6);
        QDirIterator it(archiveName, QStringList() << "*.wem", QDir::Files, QDirIterator::Subdirectories);
        QVERIFY(it.hasNext());
        while (it.hasNext())
        {
            QString filePath    = it.next();
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
        QDirIterator it(archiveName, QStringList() << "*.ogg", QDir::Files, QDirIterator::Subdirectories);
        QVERIFY(it.hasNext());
        while (it.hasNext())
        {
            QVERIFY(revorb(it.next().toStdString()));
        }
    }
}
