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
    : QObject(parent), _running(false),
      _lock(new QMutex()), _selectTimeoutValue()
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
    int selectVal = 0;

    qWarning() << "QRviNodeMonitor thread running...";

    while (_running)
    {
        resetTimevalStructure();

        {//anonymous select() scope for the locker object
            QMutexLocker l(_lock);
            selectVal = select(_socketDescriptor + 1, &_readerSockets, NULL, NULL, &_selectTimeout);
        }

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
                this->stopMonitor();
                emit rviMonitorFatalError(errno);
                break;
            }
        }

//        if (FD_ISSET(_socketDescriptor, &_readerSockets))
//        {
//            emit rviReadyRead(_socketDescriptor);
//        }
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

/*
* Per the Linux man pages of select(2), found here: http://man7.org/linux/man-pages/man2/select.2.html
* The value of our timeval structure is unreliable between utilizations within the call to select().
* Therefore, we provide this private method which must be called before every occurance of select().
*/
void QRviNodeMonitor::resetTimevalStructure()
{
    _selectTimeout.tv_sec = 0;
    _selectTimeout.tv_usec = _selectTimeoutValue;
}
