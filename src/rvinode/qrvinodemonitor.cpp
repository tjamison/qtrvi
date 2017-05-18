#include "qrvinodemonitor_p.h"

// Qt includes
#include <QDebug>
#include <QMutexLocker>
#include <QTime>
#include <QCoreApplication>

#include <errno.h>

QRviNodeMonitor::QRviNodeMonitor(QObject *parent)
    : QObject(parent), _running(false),
      _lock(new QMutex()), _socketDescriptor(0),
      _readerSocket(), _timeoutValue(0)
{
    this->setTimeout();
}

QRviNodeMonitor::QRviNodeMonitor(int fd, QObject *parent)
    : QObject(parent), _running(false), _lock(new QMutex()),
      _socketDescriptor(fd), _readerSocket(), _timeoutValue(0)
{
    this->setTimeout();
}

QRviNodeMonitor::~QRviNodeMonitor()
{
    // stop the loop
    _running = false;

    // _mutex should not be locked, safe to delete
    if (_lock)
    {
        delete _lock;
        _lock = Q_NULLPTR;
    }
}

void QRviNodeMonitor::setTimeout()
{
    /* Timeout value provided to poll() based on milliseconds */
    _timeoutValue = (50);
}

void QRviNodeMonitor::run()
{
    int result = 0;

    // build pollfd struct
    _readerSocket.fd = _socketDescriptor;
    _readerSocket.events = POLLIN;


    while (_running)
    {
        {// anonymous scope for QMutexLocker
            QMutexLocker l(_lock);
            result = poll(&_readerSocket, 1, _timeoutValue);
        }
        if (result == 0)
            continue; //timeout condition

        if (result <= -1)
        {
            //just kidding?
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                //actual error
                perror("QRviNodeMonitor::run select call error");
                this->stopMonitor();
                emit rviMonitorError(_socketDescriptor, errno);
                break;
            }
        }
        else if (result > 0)
        {
            if ((_readerSocket.revents & POLLIN) == POLLIN)
            {
                emit readyRead(_socketDescriptor);
            }
        }
    }
}

void QRviNodeMonitor::startMonitor()
{
    _running = true;
}

void QRviNodeMonitor::stopMonitor()
{
    _running = false;
}

QMutex * QRviNodeMonitor::getLock()
{
    return _lock;
}

int QRviNodeMonitor::getTimeoutValue() const
{
    return _timeoutValue;
}
