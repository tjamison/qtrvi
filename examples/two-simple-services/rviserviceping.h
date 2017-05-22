#ifndef RVISERVICEPING_H
#define RVISERVICEPING_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtRviNode/QRviServiceInterface>


class RviServicePing : public QRviServiceInterface
{
    Q_OBJECT

public:
    RviServicePing(QObject * parent = Q_NULLPTR);
    RviServicePing(int socketDescriptor, const QString &name, QObject * parent = Q_NULLPTR);
    ~RviServicePing() Q_DECL_OVERRIDE;

    void rviServiceCallback(int fd, void *serviceData, const char *parameters) Q_DECL_OVERRIDE;

private:


};

#endif // RVISERVICEPING_H
