#ifndef RVISERVICEPING_H
#define RVISERVICEPING_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtRviNode/QRviServiceInterface>


class RviServicePing : public QObject, public QRviServiceInterface
{
    Q_OBJECT

public:
    RviServicePing();
    RviServicePing(int socketDescriptor, const QString &name);

    void rviServiceCallback(int fd, void *serviceData, const char *parameters) Q_DECL_OVERRIDE;

private:


};

#endif // RVISERVICEPING_H
