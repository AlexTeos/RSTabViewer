#include "tst_psarc.h"

const QStringList archiveNames = {"stro1251_p.psarc", "somebodytold_p.psarc", "Weezer_Hero_v1_p.psarc"};

void TestPSARC::initTestCase() {}

void TestPSARC::cleanupTestCase() {}

void TestPSARC::testUnarchive()
{
    foreach(auto archiveName, archiveNames)
    {
        PSARC psarc;
        psarc.setPsarcFile(archiveName);
        QVERIFY(psarc.unarchive());
    }
}
