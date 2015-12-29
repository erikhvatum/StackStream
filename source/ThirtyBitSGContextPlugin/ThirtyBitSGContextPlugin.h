#ifndef THIRTYBITSGCONTEXTPLUGIN_H
#define THIRTYBITSGCONTEXTPLUGIN_H

#include <QGenericPlugin>


class ThirtyBitSGContextPlugin : public QGenericPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "ThirtyBitSGContextPlugin.json")
#endif // QT_VERSION >= 0x050000

public:
    ThirtyBitSGContextPlugin(QObject *parent = 0);
};

#endif // THIRTYBITSGCONTEXTPLUGIN_H
