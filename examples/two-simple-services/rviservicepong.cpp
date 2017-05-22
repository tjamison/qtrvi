#include "rviservicepong.h"

#include <QtCore/QDebug>

RviServicePong::RviServicePong(QObject * parent)
    : QRviServiceInterface(parent)
{

}

RviServicePong::RviServicePong(int socketDescriptor, const QString &name, QObject * parent)
: QRviServiceInterface(socketDescriptor, name, parent)
{

}

RviServicePong::~RviServicePong()
{

}

void RviServicePong::rviServiceCallback(int fd, void *serviceData, const char *parameters)
{
    qDebug() << "Pong service invoked!";
    qDebug() << "Parameters: " << parameters;
}
