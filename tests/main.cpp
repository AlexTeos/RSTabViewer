#include "tst_psarc.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    TESTLIB_SELFCOVERAGE_START(TestObject)
    TestPSARC tst_psarc;
    QTEST_SET_MAIN_SOURCE_PATH
    return QTest::qExec(&tst_psarc, argc, argv);
}
