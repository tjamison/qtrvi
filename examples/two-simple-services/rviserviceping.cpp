#include "rviserviceping.h"

RviServicePing::RviServicePing()
    : QRviServiceInterface()
{

}

RviServicePing::RviServicePing(int socketDescriptor, const QString &name)
    : QRviServiceInterface(socketDescriptor, name)
{

}

void RviServicePing::rviServiceCallback(int fd, void *serviceData, const char *parameters)
{

}
