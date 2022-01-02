#include "tst_psarc.h"

const QStringList archiveNames = {"stro1251_p.psarc", "somebodytold_p.psarc", "Weezer_Hero_v1_p.psarc"};

void TestPSARC::initTestCase() {}

void TestPSARC::cleanupTestCase()
{
    foreach(auto archiveName, archiveNames) { QVERIFY(psarc.unarchive(archiveName)); }
}

void TestPSARC::testUnarchive() {}
