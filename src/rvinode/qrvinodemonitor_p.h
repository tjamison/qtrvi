#ifndef QRVINODEMONITOR_H
#define QRVINODEMONITOR_H

/*******************************************************
 *          %****    PRIVATE CLASS   ****%
 * Note: This class is not part of the public interface
 * for QtRviNode. The details of this relationship may
 * change or disappear at any time for any reason and
 * will not impact the public API for a QRviNode object.
 *
********************************************************/


// Qt inlcudes
#include <QMutex>
#include <QObject>
#include <QRunnable>

// QtRvi includes
#include "qtrvinode_global.h"

// system includes
#include <sys/select.h>

QT_BEGIN_NAMESPACE

class Q_QTRVI_EXPORT QRviNodeMonitor : public QObject, public QRunnable
{
    Q_OBJECT

public:
    QRviNodeMonitor(QObject *parent = 0);
    ~QRviNodeMonitor();

    void run() override;

    void startMonitor();
    void stopMonitor();

    void addSocketDescriptor(int fd);

Q_SIGNALS:
    // success signals
    void rviReadyRead(int fd);

    // error signals
    void rviMonitorFatalError(int error);

private:
    bool       _running;
    QMutex *   _lock;
    fd_set     _readerSockets;
    int        _socketCount;
    QList<int> _fdList;
};

QT_END_NAMESPACE

#endif // QRVINODEMONITOR_H
