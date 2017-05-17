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
#include <QMutex>
#include <QObject>
#include <QRunnable>

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
    QRviNodeMonitor(int fd, QObject *parent = Q_NULLPTR);
    ~QRviNodeMonitor();

    void run() override;

    void startMonitor();
    void stopMonitor();

    QMutex * getLock();
    int getTimeoutValue() const;

Q_SIGNALS:
    void readyRead(int socket);

    // error signals
    void rviMonitorError(int socket, int error);

private:
    // private members
    bool       _running;
    QMutex *   _lock;
    int _socketDescriptor;

    // poll() objects
    struct pollfd _readerSocket;
    int _timeoutValue;
};

QT_END_NAMESPACE

#endif // QRVINODEMONITOR_H
