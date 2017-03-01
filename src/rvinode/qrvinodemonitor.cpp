#include "qrvinodemonitor_p.h"

// Qt includes
#include <QDebug>
#include <QMutexLocker>
#include <QTime>
#include <QCoreApplication>

#include <stdio.h>
#include <errno.h>

QRviNodeMonitor::QRviNodeMonitor(QObject *parent)
    : QObject(parent), _running(false), _lock(new QMutex())
{
    //init the fd_set of sockets
    FD_ZERO(&_readerSockets);
}

QRviNodeMonitor::~QRviNodeMonitor()
{
    // stop the loop
    _running = false;

    // clear reader sockets
    FD_ZERO(&_readerSockets);

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

    qWarning() << "QRviNodeMonitor thread running...";

    while (_running)
    {
        if ((selectVal = select(_socketCount + 1, &_readerSockets, NULL, NULL, NULL)) == -1)
        {
            //just kidding?
            if (errno == EINTR)
                continue;
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
            if (FD_ISSET(fd, &_readerSockets))
                emit rviReadyRead(fd);

        QTime dieTime = QTime::currentTime().addMSecs(10);
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
    ++_socketCount;
}
