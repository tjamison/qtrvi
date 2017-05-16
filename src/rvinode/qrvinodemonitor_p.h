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
#include <sys/select.h>

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

Q_SIGNALS:
    // error signals
    void rviMonitorFatalError(int error);

private:
    bool       _running;
    QMutex *   _lock;
    int _socketDescriptor;

    // this is necessary for rviProcessInput and held
    // for the lifetime of the QRviNodeMonitor in order
    // to save memory reallocation for each required call
    int * _socketDescriptorMemoryArray;
    fd_set     _readerSockets;
};

QT_END_NAMESPACE

#endif // QRVINODEMONITOR_H
