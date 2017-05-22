#include "qrviserviceinterface.h"

QRviServiceInterface::QRviServiceInterface()
{

}

QRviServiceInterface::QRviServiceInterface(int socketDescriptor, const QString &name)
    : _serviceName(QStringLiteral("")), _associatedConnection(0)
{
    _associatedConnection = socketDescriptor;
    _serviceName = name;
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
    return _associatedConnection;
}

void QRviServiceInterface::setSocketDescriptor(int socket)
{
    _associatedConnection = socket;
}
