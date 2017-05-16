#include "qrviserviceinterface.h"

QRviServiceInterface::QRviServiceInterface(QObject *parent)
    : QObject(parent)
{

}

QRviServiceInterface::QRviServiceInterface(int socketDescriptor, const QString &name, QObject *parent)
    : QObject(parent), _serviceName(name), _associatedConnection(socketDescriptor)
{

}

QRviServiceInterface::~QRviServiceInterface()
{

}


QString QRviServiceInterface::serviceName()
{
    return _serviceName;
}

void QRviServiceInterface::setServiceName(const QString &name)
{
    if (_serviceName != name)
    {
        _serviceName = name;
        emit serviceNameChanged();
    }
}

int QRviServiceInterface::getAssociatedConnection() const
{
    return _associatedConnection
}
