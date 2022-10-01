#include <QtAlgorithms>
#include <QtTest>

#include "../src/RS/psarcarchive.h"
#include "../src/RS/sng.h"

class TestPSARC : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testUnarchive();
    void testSng();
    void testSoundBank();
    void testWW2OGG();
    void testRevorb();

private:
    QStringList m_archiveNames;
};
