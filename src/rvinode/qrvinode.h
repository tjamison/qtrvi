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

    /** Function signature for RVI callback functions */
    /*typedef void (*TRviCallback) ( int fd,
    *                                void* serviceData,
    *                                const char *parameters
    *                              );
    **/
    // singleton pattern due to callback function
    // used by the rviRegisterService call
    static QRviNode * getInstance();
    static void callbackHandler(int fd, void * serviceData, const char * parameters);
    QRviServiceInterface* getServiceObjectFromMap(const QString &serviceName);
    QList<int> * activeConnections();

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

public Q_SLOTS:
    // error handlers
    void handleRviMonitorFatalError(int error);

Q_SIGNALS:
    // property signals
    void nodePortChanged();
    void nodeAddressChanged();
    void configFileChanged();

    // error signals
    void initError();
    void invalidRviHandle();
    void processInputError();
    void noConfigPathSetInEnvironment();
    void remoteConnectionError();
    void cleanupFailure(int error);
    void nodeMonitorBadPointer(int error);
    void invalidDisconnection();
    void registerServiceError(const QString serviceName, int error);
    void invokeServiceError(const QString serviceName, int error);
    void unknownErrorDuringDisconnection(int error);
    void addConnectionDuplicateFileDescriptorError();

    // success signals
    void initSuccess();
    void newActiveConnection();
    void cleanupSuccess();
    void remoteNodeConnected();
    void processInputSuccess(int fd);
    void registerServiceSuccess(const QString serviceName);
    void invokeServiceSuccess(const QString serviceName, const QString parameters);
    void disconnectSuccess(int fd);

private:

    QMap<int, QRviNodeMonitor*> _connectionReaderMap;
    TRviHandle _rviHandle;

    QString _confFile;
    QString _nodePort;
    QString _nodeAddress;

    //collection of connected services
    QMap<QString, QRviServiceInterface* > _serviceMap;

    // private methods
    void setupConnections();
    bool addNewConnectionDescriptor(int fd);

private Q_SLOTS:
    void prepareAndRunRviMonitor();

};

QT_END_NAMESPACE

#endif // QRVINODE_H

