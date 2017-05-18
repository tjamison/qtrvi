#ifndef QRVINODE_H
#define QRVINODE_H

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <rvi.h>

#include "qtrvinode_global.h"
#include "qrviserviceinterface.h"

// TODO:
// * ) Need to handle cleanup and disconnect conditions
//     from lib_rvi, currently just getting connected
//     and handling init/connect related execution cases

QT_BEGIN_NAMESPACE

class QRviNodeMonitor;

class Q_QTRVI_EXPORT QRviNode : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QRviNode)

    Q_ENUM(ERviStatus)

    Q_PROPERTY(QString configFile READ configFile WRITE setConfigFile NOTIFY configFileChanged)
    Q_PROPERTY(QString nodePort READ nodePort WRITE setNodePort NOTIFY nodePortChanged)
    Q_PROPERTY(QString nodeAddress READ nodeAddress WRITE setNodeAddress NOTIFY nodeAddressChanged)

public:

    QRviNode(QObject *parent = Q_NULLPTR);
    ~QRviNode();

    QRviServiceInterface* getServiceObjectFromMap(const QString &serviceName);

    // property readers
    QString nodePort() const;
    QString nodeAddress() const;
    QString configFile() const;

    // property writers
    void setNodePort(const QString &port);
    void setNodeAddress(const QString &address);
    void setConfigFile(const QString &file);

    // public interface, QML exposed
    Q_INVOKABLE void nodeInit();
    Q_INVOKABLE void nodeCleanup();
    Q_INVOKABLE void nodeConnect(const QString &address = QString(), const QString &port = QString());
    Q_INVOKABLE void nodeDisconnect(int fd);
    Q_INVOKABLE void registerService(const QString &serviceName,
                                     QRviServiceInterface *serviceObject,
                                     void * serviceData = Q_NULLPTR);
    Q_INVOKABLE void invokeService(const QString &serviceName, const QString &parameters = QString(QStringLiteral("{}")));
    Q_INVOKABLE int findAssociatedConnectionId(const QString &address = QString(), const QString &port = QString());

public Q_SLOTS:
    // socket watcher notifier handler
    void onReadyRead(int socket);

    // error handlers
    void handleRviMonitorError(int socket, int error);

Q_SIGNALS:
    // property signals
    void nodePortChanged();
    void nodeAddressChanged();
    void configFileChanged();

    // error signals
    void initError();
    void invalidRviHandle();
    void processInputError();
    void invalidDisconnection();
    void remoteConnectionError();
    void cleanupFailure(int error);
    void noConfigPathSetInEnvironment();
    void nodeMonitorBadPointer(int error);
    void unknownErrorDuringDisconnection(int error);
    void addConnectionDuplicateFileDescriptorError();
    void invokeServiceError(const QString serviceName, int error);
    void registerServiceError(const QString serviceName, int error);

    // success signals
    void initSuccess();
    void cleanupSuccess();
    void remoteNodeConnected();
    void newActiveConnection();
    void disconnectSuccess(int fd);
    void processInputSuccess(int fd);
    void registerServiceSuccess(const QString serviceName);
    void invokeServiceSuccess(const QString serviceName, const QString parameters);

    // node signals to affect connected services
    void signalServicesForNodeCleanup();

private:
    // rvi_lib context handle
    TRviHandle _rviHandle;

    // absolute path to rvi configuration file
    QString _confFile;

    // collection associating a given socket with it's watcher thread
    QMap<int, QRviNodeMonitor*> _connectionReaderMap;

    // data members containing open source rvi core test server address
    QString _testNodePort;
    QString _testNodeAddress;

    // collection of connected services
    QMap<QString, QRviServiceInterface* > _serviceMap;

    // private methods
    bool addNewConnectionDescriptor(int fd);
    void handleMonitorPollingFault(int socket);
};

QT_END_NAMESPACE

#endif // QRVINODE_H

