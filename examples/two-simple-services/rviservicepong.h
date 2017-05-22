#ifndef RVISERVICEPONG_H
#define RVISERVICEPONG_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtRviNode/QRviServiceInterface>


class RviServicePong : public QRviServiceInterface
{
    Q_OBJECT

public:
    RviServicePong();
    RviServicePong(int socketDescriptor, const QString &name);

    void rviServiceCallback(int fd, void *serviceData, const char *parameters) Q_DECL_OVERRIDE;

private:

};

#endif // RVISERVICEPONG_H
