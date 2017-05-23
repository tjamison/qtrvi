/*****************************************************************
 *
 * (C) 2017 Jaguar Land Rover - All Rights Reserved
 *
 * This program is licensed under the terms and conditions of the
 * Mozilla Public License, version 2.0.  The full text of the
 * Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
 *
******************************************************************/

#include "qrvinodemonitor_p.h"

// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

// system includes
#include <sys/errno.h>

QRviNodeMonitor::QRviNodeMonitor(QObject *parent)
    : QObject(parent), _running(false),
      _lock(new QMutex()), _socketDescriptor(0),
      _readerSocket(), _timeoutValue(0), _isNodeReading(false)
{
    this->resetTimeout();
}

QRviNodeMonitor::QRviNodeMonitor(int fd, const QString &address, const QString &port, QObject *parent)
    : QObject(parent), _running(false), _lock(new QMutex()),
      _socketDescriptor(fd), _socketAddress(qMakePair(address, port)),
      _readerSocket(), _timeoutValue(0), _isNodeReading(false)
{
    this->resetTimeout();
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

void QRviNodeMonitor::run()
{
    int result = 0;

    // build pollfd struct
    _readerSocket.fd = _socketDescriptor;
    _readerSocket.events = POLLIN;

    while (_running)
    {
        if (_lock->tryLock(100))
        {// we will try to get data for the time specified by timeout
            qDebug() << "QRviNodeMonitor::run() acquired the _lock";
            result = poll(&_readerSocket, 1, _timeoutValue);
            _lock->unlock();// lock will be held no longer than _timeoutValue
            qDebug() << "QRviNodeMonitor::run() released the _lock";

            if (result == 0)
                continue;// timeout condition

            if (result <= -1)
            {
                if (errno == EINTR)
                {// just kidding?
                    continue;
                }
                else
                {// actual error
                    perror("QRviNodeMonitor::run select call error");
                    this->stopMonitor();
                    emit rviMonitorError(_socketDescriptor, errno);
                    break;
                }
            }
            else if (result > 0)
            {// _readerSocket has something interesting to say
                if ((_readerSocket.revents & POLLIN) == POLLIN)
                {
                    qDebug() << "QRviNodeMonitor::run() emitting readyRead() signal";
                    _isNodeReading = true;
                    emit readyRead(_socketDescriptor);
                }
            }
        }
    }
}

void QRviNodeMonitor::resetTimeout()
{
    /* Timeout value provided to poll() based on milliseconds */
    _timeoutValue = (5);
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

QString QRviNodeMonitor::getAddress() const
{
    return _socketAddress.first;
}

QString QRviNodeMonitor::getPort() const
{
    return _socketAddress.second;
}

int QRviNodeMonitor::getSocket() const
{
    return _socketDescriptor;
}

void QRviNodeMonitor::handleNodeDoneReading()
{
    _isNodeReading = false;
}
