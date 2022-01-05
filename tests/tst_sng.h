#include <QtAlgorithms>
#include <QtTest>

#include "../src/sng.h"

class TestSNG : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testDecrypt();
};
