#ifndef THIRTYBITSGCONTEXTPLUGIN_H
#define THIRTYBITSGCONTEXTPLUGIN_H

#include "thirtybitsgcontextplugin_global.h"

#include <extensionsystem/iplugin.h>

namespace ThirtyBitSGContextPlugin {
namespace Internal {

class ThirtyBitSGContextPluginPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "ThirtyBitSGContextPlugin.json")

public:
    ThirtyBitSGContextPluginPlugin();
    ~ThirtyBitSGContextPluginPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:
    void triggerAction();
};

} // namespace Internal
} // namespace ThirtyBitSGContextPlugin

#endif // THIRTYBITSGCONTEXTPLUGIN_H
