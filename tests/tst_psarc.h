#include <QtAlgorithms>
#include <QtTest>

#include "../src/psarc.h"

class TestPSARC : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testUnarchive();

private:
    PSARC psarc;
};
