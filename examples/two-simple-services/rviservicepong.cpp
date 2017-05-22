#include "rviservicepong.h"

RviServicePong::RviServicePong()
    : QRviServiceInterface()
{

}

RviServicePong::RviServicePong(int socketDescriptor, const QString &name)
: QRviServiceInterface(socketDescriptor, name)
{

}

void RviServicePong::rviServiceCallback(int fd, void *serviceData, const char *parameters)
{

}
