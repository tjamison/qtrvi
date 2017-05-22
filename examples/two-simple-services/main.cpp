#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QDebug>

#include <QtRviNode/QRviNode>

#include "rviserviceping.h"
#include "rviservicepong.h"

int main(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);

    qputenv("QT_RVI_NODE_CONFIG_FILE", "./conf.json");

    QRviNode node;

    node.nodeInit();

    int socket = node.nodeConnect();

    QObject::connect(&node, &QRviNode::registerServiceSuccess, [](){
        qDebug() << "Registered a service!";
    });

    RviServicePing * pinger = new RviServicePing(socket, "pinger");
    RviServicePong * ponger = new RviServicePong(socket, "ponger");

    node.registerService(pinger->serviceName(), pinger);
    node.registerService(ponger->serviceName(), ponger);

    return app.exec();
}
