#ifndef RVISERVICEPONG_H
#define RVISERVICEPONG_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtRviNode/QRviServiceInterface>


class RviServicePong : public QRviServiceInterface
{
    Q_OBJECT

public:
    RviServicePong(QObject * parent = Q_NULLPTR);
    RviServicePong(int socketDescriptor, const QString &name, QObject * parent = Q_NULLPTR);
    ~RviServicePong() Q_DECL_OVERRIDE;

    void rviServiceCallback(int fd, void *serviceData, const char *parameters) Q_DECL_OVERRIDE;

private:

};

#endif // RVISERVICEPONG_H
