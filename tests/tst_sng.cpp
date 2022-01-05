#include "tst_sng.h"

const QStringList sngNames = {"weezerhero_lead.sng", "somebodytold_lead.sng", "stro1251_lead.sng"};

void TestSNG::initTestCase() {}

void TestSNG::cleanupTestCase() {}

void TestSNG::testDecrypt()
{
    foreach(auto sngName, sngNames)
    {
        SNG sng;
        sng.setSngFile(sngName);
        QVERIFY(sng.decrypt());
    }
}
