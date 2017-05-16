#include "qrviserviceinterface.h"

QRviServiceInterface::QRviServiceInterface(QObject *parent)
    : QObject(parent)
{

}

QRviServiceInterface::QRviServiceInterface(const QString &name, QObject *parent)
    : QObject(parent), _serviceName(name)
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
