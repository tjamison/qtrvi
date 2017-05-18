#ifndef QRVINODEMONITOR_H
#define QRVINODEMONITOR_H

/*******************************************************
 *          %****    PRIVATE CLASS   ****%
 * Note: This class is not part of the public interface
 * for QtRviNode. The details of this relationship may
 * change or disappear at any time for any reason and
 * will not impact the public API for a QRviNode object.
 *
 * We mean it.
 *
********************************************************/


// Qt inlcudes
#include <QtCore/QPair>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QRunnable>

// QtRvi includes
#include "qtrvinode_global.h"

// system includes
#include <sys/poll.h>

QT_BEGIN_NAMESPACE

class Q_QTRVI_EXPORT QRviNodeMonitor : public QObject, public QRunnable
{
    Q_OBJECT

public:
    QRviNodeMonitor(QObject *parent = Q_NULLPTR);
    QRviNodeMonitor(int fd, const QString &address, const QString &port, QObject *parent = Q_NULLPTR);
    ~QRviNodeMonitor();

    void run() Q_DECL_OVERRIDE;

    // control thread execution
    void startMonitor();
    void stopMonitor();

    // synchronize with QRviNode user object
    QMutex * getLock();
    int getTimeoutValue() const;
    QString getAddress() const;
    QString getPort() const;

Q_SIGNALS:
    void readyRead(int socket);

    // error signals
    void rviMonitorError(int socket, int error);

private:
    // private members
    bool       _running;
    QMutex *   _lock;
    int _socketDescriptor;
    QPair<QString, QString> _socketAddress;

    // poll() objects
    struct pollfd _readerSocket;
    int _timeoutValue;

    void setTimeout();
};

QT_END_NAMESPACE

#endif // QRVINODEMONITOR_H
