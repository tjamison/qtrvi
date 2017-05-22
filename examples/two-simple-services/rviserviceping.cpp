#include "rviserviceping.h"

#include <QtCore/QDebug>

RviServicePing::RviServicePing(QObject * parent)
    : QRviServiceInterface(parent)
{

}

RviServicePing::RviServicePing(int socketDescriptor, const QString &name, QObject * parent)
: QRviServiceInterface(socketDescriptor, name, parent)
{

}

RviServicePing::~RviServicePing()
{

}

void RviServicePing::rviServiceCallback(int fd, void *serviceData, const char *parameters)
{
    qDebug() << "Ping service invoked!";
    qDebug() << "Parameters: " << parameters;
}
