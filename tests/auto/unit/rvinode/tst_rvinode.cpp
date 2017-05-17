#include <QtTest/QtTest>
#include <QtRviNode/QRviNode>

class TestQRviNode : public QObject
{
    Q_OBJECT

    friend class QRviNode;

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();

    void testRviNodeInitWithNoConfigFilePath();
    void testRviNodeInitWithBadConfigFilePath();
    void testRviNodeInitWithGoodConfigFilePath();

private:
    QRviNode * node;
};

void TestQRviNode::initTestCase()
{
}

void TestQRviNode::init()
{
    node = new QRviNode(this);
}

void TestQRviNode::testRviNodeInitWithGoodConfigFilePath()
{
    QSignalSpy initSuccessSpy(node, &QRviNode::initSuccess);
    QSignalSpy initErrorSpy(node, &QRviNode::initError);
    QSignalSpy noConfigPathSetSpy(node, &QRviNode::noConfigPathSetInEnvironment);

    qputenv("QT_RVI_NODE_CONFIG_FILE", "./conf.json");

    node->nodeInit();

    // initSuccess signal should only be emitted once
    QCOMPARE(initSuccessSpy.count(), 1);

    // initError should not be emitted
    QCOMPARE(initErrorSpy.count(), 0);

    // we should not see the no config path error
    QCOMPARE(noConfigPathSetSpy.count(), 0);
}

void TestQRviNode::testRviNodeInitWithBadConfigFilePath()
{
    QSignalSpy initSuccessSpy(node, &QRviNode::initSuccess);
    QSignalSpy initErrorSpy(node, &QRviNode::initError);
    QSignalSpy noConfigPathSetSpy(node, &QRviNode::noConfigPathSetInEnvironment);

    qputenv("QT_RVI_NODE_CONFIG_FILE", ".");

    node->nodeInit();

    // initSuccess should not be emitted
    QCOMPARE(initSuccessSpy.count(), 0);

    // initError should be emitted only once
    QCOMPARE(initErrorSpy.count(), 1);

    // we should not see the no config path error
    QCOMPARE(noConfigPathSetSpy.count(), 0);
}

void TestQRviNode::testRviNodeInitWithNoConfigFilePath()
{
    QSignalSpy initSuccessSpy(node, &QRviNode::initSuccess);
    QSignalSpy initErrorSpy(node, &QRviNode::initError);
    QSignalSpy noConfigPathSetSpy(node, &QRviNode::noConfigPathSetInEnvironment);

    // no environment variable for QT_RVI_NODE_CONFIG_FILE is set here

    node->nodeInit();

    // initSuccess should not be emitted
    QCOMPARE(initSuccessSpy.count(), 0);

    // initError should not be emitted
    QCOMPARE(initErrorSpy.count(), 0);

    // we should only see the no config path error once
    QCOMPARE(noConfigPathSetSpy.count(), 1);
}

void TestQRviNode::cleanupTestCase()
{
}

void TestQRviNode::cleanup()
{
    if (node)
    {
        delete node;
        node = Q_NULLPTR;
    }
}

QTEST_MAIN(TestQRviNode)
#include "tst_rvinode.moc"
