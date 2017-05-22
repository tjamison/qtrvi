#include "rviservicepong.h"

RviServicePong::RviServicePong()
    : QRviServiceInterface(this)
{

}

RviServicePong::RviServicePong(int socketDescriptor, const QString &name)
: QRviServiceInterface(socketDescriptor, name, this)
{

}

void RviServicePong::rviServiceCallback(int fd, void *serviceData, const char *parameters)
{

}
