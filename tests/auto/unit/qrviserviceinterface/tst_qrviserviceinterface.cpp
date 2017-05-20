#include <QtTest/QtTest>
#include <QtRviNode/QtRviNode>

class TestQRviServiceInterface : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();

private:
    QRviNode * node;
};

void TestQRviServiceInterface::initTestCase()
{

}

void TestQRviServiceInterface::init()
{

}

void TestQRviServiceInterface::cleanupTestCase()
{

}

void TestQRviServiceInterface::cleanup()
{

}


QTEST_MAIN(TestQRviServiceInterface)
#include "tst_qrviserviceinterface.moc"
