#include "qrvinodemonitor_p.h"

// Qt includes
#include <QDebug>
#include <QMutexLocker>
#include <QTime>
#include <QCoreApplication>

#include <stdio.h>
#include <errno.h>
#include "time.h"

QRviNodeMonitor::QRviNodeMonitor(QObject *parent)
    : QObject(parent), _running(false), _lock(new QMutex())
{
    qDebug() << "Constructing a new node monitor.";
    //init the fd_set of sockets
    FD_ZERO(&_readerSockets);
    _maxFd = 0;
}

QRviNodeMonitor::~QRviNodeMonitor()
{
    // stop the loop
    _running = false;

    // clear reader sockets
    FD_ZERO(&_readerSockets);

    qDebug() << "Just cleared the sockets. Value now? " << _maxFd;

    // _mutex should not be locked, safe to delete
    if (_lock)
    {
        delete _lock;
        _lock = Q_NULLPTR;
    }
}

void QRviNodeMonitor::run()
{
    int selectVal = 0;
    int msToWait = 10;

    qWarning() << "QRviNodeMonitor thread running...";

    while (_running)
    {
        selectVal = select(_maxFd + 1, &_readerSockets, NULL, NULL, NULL); // no timeout, block until read is available
        if (selectVal == -1)
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
                _running = false;
                emit rviMonitorFatalError(errno);
                break;
            }
        }

        for (int fd : _fdList)
        {
            if (FD_ISSET(fd, &_readerSockets))
            {
                emit rviReadyRead(fd);
            }
        }

        QTime dieTime = QTime::currentTime().addMSecs(msToWait);
        while (QTime::currentTime() < dieTime);
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

void QRviNodeMonitor::addSocketDescriptor(int fd)
{
    QMutexLocker l(_lock);
    _fdList.append(fd);
    FD_SET(fd, &_readerSockets);
    if (fd > _maxFd)
        _maxFd = fd;
}
