/*****************************************************************
 *
 * (C) 2017 Jaguar Land Rover - All Rights Reserved
 *
 * This program is licensed under the terms and conditions of the
 * Mozilla Public License, version 2.0.  The full text of the
 * Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
 *
******************************************************************/

#ifndef QRVINODEMONITOR_H
#define QRVINODEMONITOR_H

// Qt inlcudes
#include <QtCore/QPair>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QRunnable>

// QtRvi includes
#include "qtrvinode_global.h"

// system includes
#include <sys/poll.h>


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
QT_BEGIN_NAMESPACE

class QRviNodeMonitor : public QObject, public QRunnable
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
    int getSocket() const;

public Q_SLOTS:
    void handleNodeDoneReading();

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

    // node sync boolean
    bool _isNodeReading;

    void resetTimeout();
};

QT_END_NAMESPACE

#endif // QRVINODEMONITOR_H
