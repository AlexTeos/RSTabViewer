#include <QtAlgorithms>
#include <QtTest>

#include "../src/RS/psarc.h"
#include "../src/RS/sng.h"

class TestPSARC : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testUnarchive();
    void testDecrypt();
    void testParse();
    void testWW2OGG();
    void testRevorb();

private:
    QStringList m_archiveNames;
};
