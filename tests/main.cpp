#include "tst_psarc.h"
#include "tst_sng.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    TESTLIB_SELFCOVERAGE_START(TestObject)
    TestPSARC tst_psarc;
    TestSNG   tst_sng;
    QTEST_SET_MAIN_SOURCE_PATH
    return QTest::qExec(&tst_psarc, argc, argv) or QTest::qExec(&tst_sng, argc, argv);
}
