#ifndef QRVINODE_H
#define QRVINODE_H

#include <QMap>
#include <QObject>
#include <QString>

#include <rvi.h>

#include "qtrvinode_global.h"
#include "qrviserviceobject.h"

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

    QRviNode(QObject *parent = 0);
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
    QRviServiceObject* getServiceObjectFromMap(const QString &serviceName);
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
                                     QRviServiceObject *serviceObject,
                                     void * serviceData = Q_NULLPTR);

public Q_SLOTS:
    // success handlers
    void processInput(int fd);

    // error handlers
    void handleRviMonitorFatalError(int error);

Q_SIGNALS:

    // property signals
    void nodePortChanged();
    void nodeAddressChanged();
    void configFileChanged();

    // error signals
    void rviBadHandle();
    void rviInitFailure();
    void invalidRviNodeHandle();
    void rviProcessInputFailure();
    void rviNodeNoPathSetInEnvironment();
    void rviRemoteNodeConnectionFailure();
    void rviNodeCleanupFailure(int error);
    void rviNodeMonitorBadPointer(int error);
    void rviNodeDisconnectInvalidConnection();
    void rviNodeRegisterServiceError(int error);
    void rviNodeDisconnectUnknownError(int error);
    void rviNodeConnectionDuplicateFileDescriptor();

    // success signals
    void rviInitialized();
    void newActiveConnection();
    void rviNodeCleanupSuccess();
    void rviRemoteNodeConnected();
    void rviProcessInputSuccess();
    void rviNodeRegisterServiceSuccess();
    void rviNodeDisconnectionSuccess(int fd);

private:

    QList<int> _activeConnections;
    TRviHandle _rviHandle;

    QString _confFile;
    QString _nodePort;
    QString _nodeAddress;

    QRviNodeMonitor * _monitor;

    //collection of connected services
    QMap<QString, QRviServiceObject* > _serviceMap;

    // private methods
    void setupConnections();
    bool addNewConnectionDescriptor(int fd);

private Q_SLOTS:
    void prepareAndRunRviMonitor();

};

QT_END_NAMESPACE

#endif // QRVINODE_H

