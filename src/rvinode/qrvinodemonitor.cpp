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
}

QRviNodeMonitor::QRviNodeMonitor(int fd, QObject *parent)
    : QObject(parent), _socketDescriptor(fd), _socketDescriptorMemoryArray(Q_NULLPTR)
{
    //c-style allocation for use with rvi_lib c api
    _socketDescriptorMemoryArray = (int*)malloc(sizeof(int*));
    _socketDescriptorMemoryArray[0] = _socketDescriptor;
}

QRviNodeMonitor::~QRviNodeMonitor()
{
    // stop the loop
    _running = false;

    // clear reader sockets
    FD_ZERO(&_readerSockets);

    if (_socketDescriptorMemoryArray)
    {//c-style free to match c-style allocation
        free(_socketDescriptorMemoryArray);
        _socketDescriptorMemoryArray = Q_NULLPTR;
    }

    // _mutex should not be locked, safe to delete
    if (_lock)
    {
        delete _lock;
        _lock = Q_NULLPTR;
    }
}

void QRviNodeMonitor::run()
{
//    int selectVal = 0;
//    int msToWait = 200;

    qWarning() << "QRviNodeMonitor thread running...";

    while (_running)
    {

        /*selectVal = select(_maxFd + 1, &_readerSockets, NULL, NULL, NULL); // no timeout, block until read is available
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
        while (QTime::currentTime() < dieTime);*/
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
