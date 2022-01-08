#include "tst_sng.h"

const QStringList sngNames          = {"weezerhero_lead.sng", "somebodytold_lead.sng", "stro1251_lead.sng"};
const QStringList sngDecryptedNames = {"stro1251_lead_uncompressed.sng",
                                       "weezerhero_lead_uncompressed.sng",
                                       "somebodytold_lead_uncompressed.sng"};

void TestSNG::initTestCase() {}

void TestSNG::cleanupTestCase() {}

void TestSNG::testDecrypt()
{
    //foreach(auto sngName, sngNames)
    //{
    //    SNG sng;
    //    sng.setSngFile(sngName);
    //    QVERIFY(sng.decrypt());
    //}
}

void TestSNG::testParse()
{
    foreach(auto sngDecryptedName, sngDecryptedNames)
    {
        SNG sng;
        sng.setSngDecryptedFile(sngDecryptedName);
        sng.parse();
    }
}
