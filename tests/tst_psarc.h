#include <QtAlgorithms>
#include <QtTest>

#include "../src/psarc.h"
#include "../src/sng.h"

class TestPSARC : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testUnarchive();
    void testDecrypt();
    void testParse();
    void testGetTablature();

private:
    QStringList  m_archiveNames;
    QVector<SNG> m_parsedSng;
};
