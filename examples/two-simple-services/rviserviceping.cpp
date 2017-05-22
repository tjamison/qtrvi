#include "rviserviceping.h"

RviServicePing::RviServicePing()
    : QRviServiceInterface(this)
{

}

RviServicePing::RviServicePing(int socketDescriptor, const QString &name)
    : QRviServiceInterface(socketDescriptor, name, this)
{

}

void RviServicePing::rviServiceCallback(int fd, void *serviceData, const char *parameters)
{

}
