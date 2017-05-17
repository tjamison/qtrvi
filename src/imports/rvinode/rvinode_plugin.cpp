/*
*
* (C) 2017 Jaguar Land Rover
*
*/

#include <QtRviNode/QtRviNode>
#include <QtQml/qqmlextensionplugin.h>

QT_BEGIN_NAMESPACE

class RviNodePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("RviNode"));

    }
};

QT_END_NAMESPACE

#include "rvinode_plugin.moc"
