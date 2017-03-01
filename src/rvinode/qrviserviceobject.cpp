#include "qrviserviceobject.h"

QRviServiceObject::QRviServiceObject(QObject *parent)
    : QObject(parent)
{

}

QRviServiceObject::QRviServiceObject(const QString &name, QObject *parent)
    : QObject(parent), _serviceName(name)
{

}

QRviServiceObject::~QRviServiceObject()
{

}


QString QRviServiceObject::serviceName()
{
    return _serviceName;
}

void QRviServiceObject::setServiceName(const QString &name)
{
    if (_serviceName != name)
    {
        _serviceName = name;
        emit serviceNameChanged();
    }
}
