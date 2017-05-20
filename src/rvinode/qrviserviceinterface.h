#ifndef QRVISERVICEINTERFACE_H
#define QRVISERVICEINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QtPlugin>

#include "qtrvinode_global.h"

QT_BEGIN_NAMESPACE

class Q_QTRVI_EXPORT QRviServiceInterface : public QObject
{
    Q_OBJECT

    Q_DISABLE_COPY(QRviServiceInterface)

    Q_PROPERTY(QString serviceName READ serviceName WRITE setServiceName NOTIFY serviceNameChanged)

public:
    QRviServiceInterface(QObject *parent = Q_NULLPTR);
    QRviServiceInterface(int socketDescriptor, const QString &name, QObject *parent = Q_NULLPTR);
    ~QRviServiceInterface();

    virtual void rviServiceCallback(int fd, void * serviceData, const char * parameters) = 0;
    virtual void handleNodeCleanupSignal() = 0;

    QString serviceName();
    int getAssociatedConnection() const;

    void setServiceName(const QString &name);
    void setSocketDescriptor(int socket);

Q_SIGNALS:
    void serviceNameChanged();

private:
    QString _serviceName;
    int _associatedConnection;

};

#define QRviServiceInterface_iid "com.genivi.qtrvi.QRviServiceInterface/1.0"
Q_DECLARE_INTERFACE(QRviServiceInterface, QRviServiceInterface_iid)

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QRviServiceInterface*)

#endif //QRVISERVICEINTERFACE_H
