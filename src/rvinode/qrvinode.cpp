#include "qrvinode.h"

#include <QtCore/QDir>
#include <QtCore/QThreadPool>
#include <QtCore/QVector>
#include <QtCore/QDebug>

#include "qrvinodemonitor_p.h"

/** start rvi_lib callback helpers **/
typedef struct CallbackData {
    QString _serviceName;
    QRviNode * _node;

    CallbackData(const QString &name, QRviNode * node)
        : _serviceName(name), _node(node) {}

} CallbackData;

void callbackHandler(int fd, void *serviceData, const char * parameters);
/** end rvi_lib callback helpers **/

// Constructor
QRviNode::QRviNode(QObject *parent)
    : QObject(parent), _rviHandle(NULL),
      _confFile(QStringLiteral("")), _nodePort(QStringLiteral("9007")),
      _nodeAddress(QStringLiteral("38.129.64.41"))
{
    //    setupConnections();
}

void QRviNode::setupConnections()
{
    //    connect(this, &QRviNode::nodeMonitorBadPointer,
    //            this, &QRviNode::handleRviMonitorFatalError);

    //    connect(_monitor, &QRviNodeMonitor::rviMonitorFatalError,
    //            this, &QRviNode::handleRviMonitorFatalError);

    //    connect(_monitor, &QRviNodeMonitor::bytesAvailable,
    //            this, &QRviNode::processBytes);
}

// Initializer method
void QRviNode::nodeInit()
{
    if (_rviHandle)
    {
        qWarning() << "Error: This QRviNode(" << this << ") was already initialized.";
        return;
    }
    // check for path to configuration
    _confFile = QDir::fromNativeSeparators(qgetenv("QT_RVI_NODE_CONFIG_FILE"));
    if (_confFile.isEmpty())
    {
        qWarning() << "Error: QT_RVI_NODE_CONFIG_FILE must be set in order to use QRviNode";
        emit noConfigPathSetInEnvironment();
        return;
    }
    // init API and get handle
    _rviHandle = rviInit(_confFile.toLatin1().data());
    if (!_rviHandle)
    {
        qWarning() << "Error: rviInit failed to return a valid API handle";
        emit initError();
        return;
    }
    emit initSuccess();
}

// Cleanup method
void QRviNode::nodeCleanup()
{
    int returnVal = 0;

    //memory and connections to this handle
    //are cleaned up by rvi_lib
    returnVal = rviCleanup(_rviHandle);
    if (returnVal != 0)
    {
        qWarning() << "Error: rviCleanup returned a non-zero value"
                   << "Error Val: " << returnVal;
        emit cleanupFailure(returnVal);
    }
    emit cleanupSuccess();
}

// Rvi Node connection method
// Takes address and port as params but default arg is ""
// uses the existing member values for address/port if no
// new values are passed to the method
void QRviNode::nodeConnect(const QString &address, const QString &port)
{
    int fd = 0;

    // did we get new connection info?
    if (!address.isEmpty())
        setNodeAddress(address);
    if (!port.isEmpty())
        setNodePort(port);

    // check the handle is valid
    if (_rviHandle)
    {
        // connect to rvi node
        fd = rviConnect(_rviHandle, _nodeAddress.toStdString().c_str(),
                        _nodePort.toStdString().c_str());
        // check for valid file descriptor
        if (fd < 0)
        {
            qWarning() << "Error: rviConnect failed to return a valid API handle"
                       << "Please check the server address and port"
                       << "Address: " << _nodeAddress << ":" << _nodePort;
            emit remoteConnectionError();
        }
        if (addNewConnectionDescriptor(fd))
            emit remoteNodeConnected();
    }
    else
    {
        qWarning() << "Error: invalid RviHandle, is the node properly initialized?";
        emit invalidRviHandle();
    }
}

// Rvi Node disconnection method
// checks the list of connections for the descriptor received
// and, if valid, removes and disconnects the specified descriptor
void QRviNode::nodeDisconnect(int fd)
{
    int returnVal = 0;

    // is this a valid disconnect request?
    if (!_connectionReaderMap.contains(fd))
    {
        qWarning() << "Error: specified connection does not exist in the list"
                   << "of active connections"
                   << "Specified connection: " << fd;
        emit invalidDisconnection();
        return;
    }

    // take and cleanup the related monitor thread
    auto * m = _connectionReaderMap[fd];
    if (m)
    {
        m->stopMonitor();

        // wait the timeout length of the monitor thread before deleting
        QTime dieTime = QTime::currentTime().addMSecs(m->getTimeoutValue());
        while (QTime::currentTime() < dieTime);

        delete m;
        m = Q_NULLPTR;
    }
    // then remove the descriptor map entry
    _connectionReaderMap.remove(fd);

    // finally tell rvi_lib we no longer need this
    returnVal = rviDisconnect(_rviHandle, fd);

    if (returnVal != 0)
    {
        qWarning() << "Error: unknown failure from rviDisconnect call"
                   << "Error Value: " << returnVal;
        emit unknownErrorDuringDisconnection(returnVal);
        return;
    }
    emit disconnectSuccess(fd);
}

// TODO: serviceData param? needs a couple overloads possibly?
void QRviNode::registerService(const QString &serviceName, QRviServiceInterface *serviceObject, void *serviceData)
{
    Q_UNUSED(serviceData)

    int result = 0;

    connect(this, &QRviNode::signalServicesForNodeCleanup,
            serviceObject, &QRviServiceInterface::handleNodeCleanupSignal);

    // save the serviceObject pointer
    _serviceMap[serviceName] = serviceObject;

    CallbackData data(serviceName, this);

    result = rviRegisterService(_rviHandle, serviceName.toLocal8Bit().data(), callbackHandler, &data);
    if (result != 0)
    {
        qWarning() << "Error: unknown failure from rviRegisterService call"
                   << "Error Value: " << result;
        emit registerServiceError(serviceName, result);
        return;
    }
    emit registerServiceSuccess(serviceName);
}

void callbackHandler(int fd, void *serviceData, const char *parameters)
{
    CallbackData * d = (CallbackData*)serviceData;
    d->_node->getServiceObjectFromMap(d->_serviceName)
            ->rviServiceCallback(fd,
                                 serviceData,
                                 parameters);
}

void QRviNode::invokeService(const QString &serviceName, const QString &parameters)
{
    if (serviceName.isEmpty())
    {
        qWarning() << "Error: cannot invoke a service without a service name parameter";
        return;
    }

    int result = 0;
    QVector<QMutexLocker *> lockers;

    // gather all reader locks
    for (QRviNodeMonitor * m : _connectionReaderMap)
        lockers.push_back(new QMutexLocker(m->getLock()));

    // now thread safe to invoke write on unknown socket connection
    result = rviInvokeService(_rviHandle, serviceName.toLocal8Bit().data(), parameters.toLocal8Bit().data());

    // release all memory, automatically freeing all locks
    for (auto * l : lockers)
        delete l;

    if (result != 0)
    {
        qWarning() << "Error: unknown failure from rviInvokeService call"
                   << "Error Value: " << result;
        emit invokeServiceError(serviceName, result);
        return;
    }
    emit invokeServiceSuccess(serviceName, parameters);
}

void QRviNode::onReadyRead(int socket)
{
    // create int* of lenth = 1
    int * connectionArray = (int*)malloc(sizeof(int *));

    // assign the only element of connectionArray
    connectionArray[0] = socket;

    int result = rviProcessInput(_rviHandle, connectionArray, 1);

    if (result != 0)
    {
        qWarning() << "Unexpected error from rviProcessInput"
                   << "Error value: " << result;
        emit processInputError();
        return;
    }
    emit processInputSuccess(socket);
    free(connectionArray);
}

QRviServiceInterface * QRviNode::getServiceObjectFromMap(const QString &serviceName)
{
    if (_serviceMap.contains(serviceName))
        return _serviceMap[serviceName];
    return Q_NULLPTR;
}

/* Property methods */

// Returns the string of nodePort
QString QRviNode::nodePort() const
{
    return _nodePort;
}

// Sets the node port to the new value if it is actually new
void QRviNode::setNodePort(const QString &port)
{
    if (_nodePort != port)
    {
        _nodePort = port;
        emit nodePortChanged();
    }
}

// Returns the string of nodeAddress
QString QRviNode::nodeAddress() const
{
    return _nodeAddress;
}

// Sets the node address to the new ip if it is actually new
void QRviNode::setNodeAddress(const QString &address)
{
    if (_nodeAddress != address)
    {
        _nodeAddress = address;
        emit nodeAddressChanged();
    }
}

// Returns the string of the config file
QString QRviNode::configFile() const
{
    return _confFile;
}

// Sets the config file to the new value if it is actually new
void QRviNode::setConfigFile(const QString &file)
{
    if (_confFile != file)
    {
        _confFile = file;
        emit configFileChanged();
    }
}

QRviNode::~QRviNode()
{
    // stop all threads before the nodeCleanup call
    for (auto * m : _connectionReaderMap)
    {
        if (m)
        {
            m->stopMonitor();

            // wait the timeout length of the monitor thread before deleting
            QTime dieTime = QTime::currentTime().addMSecs(m->getTimeoutValue());
            while (QTime::currentTime() < dieTime);

            delete m;
            m = Q_NULLPTR;
        }
    }

    // short wait time just to make sure everyone has returned
    QThreadPool::globalInstance()->waitForDone(100);

    _connectionReaderMap.clear();
    this->nodeCleanup();
    emit signalServicesForNodeCleanup();
}

/* Private methods */

// adds a new descriptor to the self-managed connection list
// checks for duplicate descriptor values, as this should not happen
// return true if unique descriptor added and notifies
// return false if unique descriptor added and notifies
bool QRviNode::addNewConnectionDescriptor(int fd)
{
    // not allowed to have duplicate descriptors
    if (_connectionReaderMap.contains(fd))
    {
        qWarning() << "Error: QRviNode expects new connections to"
                   << "receive a unique integer file descriptor";
        emit addConnectionDuplicateFileDescriptorError();
        return false;
    }

    _connectionReaderMap.insert(fd, new QRviNodeMonitor(fd, this));

    // pull, prepare, and start current monitor object
    auto * m = _connectionReaderMap[fd];

    // make connections to the current monitor
    connect(m, &QRviNodeMonitor::readyRead,
            this, &QRviNode::onReadyRead);
    connect(m, &QRviNodeMonitor::rviMonitorError,
            this, &QRviNode::handleRviMonitorError);

    // start the thread
    m->startMonitor();
    QThreadPool::globalInstance()->start(m);

    emit newActiveConnection();
    return true;
}

/* *
 * This method switches over the errno result signaled by
 * the QRviNodeMonitor thread, which provides the associated
 * socket descriptor and the error code in order to check
 * which operations should be performed.
 * */
void QRviNode::handleRviMonitorError(int socket, int error)
{
    switch (error)
    {
    case EFAULT:
        qWarning() << "Fatal Error: array given as argument to poll()"
                   << "was not contained in the calling program's address space!";
        this->nodeDisconnect(socket);
        break;
    case EINVAL:
        // should never see this because we only use poll() on a single socket
        qWarning() << "Fatal Error: nfds param value exceeds RLIMIT_NOFILE value.";
        break;
    case ENOMEM:
        qWarning() << "Fatal Error: system has no space remaining to allocate"
                   << "the file descriptor tables!";
        this->nodeDisconnect(socket);
        break;
    }
}
