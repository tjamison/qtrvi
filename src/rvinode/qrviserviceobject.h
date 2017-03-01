#ifndef QRVISERVICEOBJECT_H
#define QRVISERVICEOBJECT_H

#include <QObject>
#include <QString>

#include "qtrvinode_global.h"

QT_BEGIN_NAMESPACE

class Q_QTRVI_EXPORT QRviServiceObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString serviceName READ serviceName WRITE setServiceName NOTIFY serviceNameChanged)

public:
    QRviServiceObject(QObject *parent = 0);
    QRviServiceObject(const QString &name, QObject *parent = 0);
    ~QRviServiceObject();

    virtual void rviServiceCallback(int fd, void * serviceData, const char * parameters) = 0;

    QString serviceName();

    void setServiceName(const QString &name);

Q_SIGNALS:
    void serviceNameChanged();

private:
    QString _serviceName;

};

QT_END_NAMESPACE

#endif //QRVISERVICEOBJECT_H
