/*
*
* (C) 2017 Jaguar Land Rover
*
*/

#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/QQmlEngine>

#include <QtRviNode/QtRviNode>

QT_BEGIN_NAMESPACE

class RviNodePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("RviNode"));

        qmlRegisterType<QRviNode>(uri, 1, 0, "RviNode");
        qmlRegisterUncreatableType<QRviServiceInterface>(uri, 1, 0, "RviServiceInterface", "Pure virtual interface class");
    }
};

QT_END_NAMESPACE

#include "rvinode_plugin.moc"
