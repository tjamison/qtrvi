#include <QtTest/QtTest>
//#include <QtRviNode/QRviNode>

class TestQRviNode : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

private:
};

void TestQRviNode::initTestCase()
{
    Q_ASSERT(true);
}

void TestQRviNode::cleanupTestCase()
{
    Q_ASSERT(true);
}

QTEST_MAIN(TestQRviNode)
#include "tst_rvinode.moc"
