#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QDebug>

#include <QtRviNode/QRviNode>

#include "rviserviceping.h"
#include "rviservicepong.h"

int main(int argc, char * argv[])
{
    int counter = 0;
    QString servicePrefix(QStringLiteral("genivi.org/client/bbfbb478-d628-480a-8528-cff40d73678f/"));
    QCoreApplication app(argc, argv);

    qputenv("QT_RVI_NODE_CONFIG_FILE", "./conf.json");

    QRviNode node;

    // Connect a few lambdas to log events
    QObject::connect(&node, &QRviNode::initSuccess, [](){
        qDebug() << "Successful init!";
    });
    QObject::connect(&node, &QRviNode::remoteNodeConnected, [](){
        qDebug() << "Remote node connected!";
    });
    QObject::connect(&node, &QRviNode::initError, [](){
        qDebug() << "Error init'ing the node!";
    });
    QObject::connect(&node, &QRviNode::remoteConnectionError, [](){
        qDebug() << "Error connecting to specified remote address!";
    });
    QObject::connect(&node, &QRviNode::registerServiceSuccess, [](){
        qDebug() << "Registered a service!";
    });

    node.nodeInit();

    int socket = node.nodeConnect();

    RviServicePing * pinger = new RviServicePing(socket, "pinger");

    RviServicePong * ponger = new RviServicePong(socket, "ponger");

    // Connect to service invoke service signals
    QObject::connect(&node, &QRviNode::invokeServiceError,
                     []() {
        qDebug() << "Currently, rvi_lib does not support local service invokations."
                 << "This error signal signifies success.";
    });

    // Connect to service destroy signals
    QObject::connect(pinger, &RviServicePing::destroyRviService,
                     [&]() {
        delete pinger;
    });
    QObject::connect(ponger, &RviServicePong::destroyRviService,
                     [&]() {
        delete ponger;
    });

    node.registerService(pinger->serviceName(), pinger);
    node.registerService(ponger->serviceName(), ponger);

    node.invokeService(servicePrefix + pinger->serviceName());
    node.invokeService(servicePrefix + ponger->serviceName());

    qDebug() << "Program counter: " << counter++;
    // Know when the app is ending to cleanup memory
    QObject::connect(&app, &QCoreApplication::aboutToQuit,
                     [&]() {
        node.nodeDisconnect(socket);
    });

    qDebug() << "Program counter: " << counter++;
    return app.exec();
}
