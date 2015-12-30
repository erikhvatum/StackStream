#pragma once

#include <QtCore/qplugin.h>
#include <QtQuick/private/qsgcontext_p.h>
#include <QtQuick/private/qsgcontextplugin_p.h>
#include <QObject>

class QSGContextFactoryInterface;

class ThirtyBitSGContextPlugin
  : public QObject,
    public QSGContextFactoryInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QSGContextFactoryInterface" FILE "ThirtyBitSGContextPlugin.json")
    Q_INTERFACES(QSGContextFactoryInterface:QFactoryInterface)

public:
    ThirtyBitSGContextPlugin(QObject *parent = 0);

    QSGContext* create(const QString& key) const override;
    QStringList keys() const override;
    virtual QQuickTextureFactory *createTextureFactoryFromImage(const QImage &) override { return 0; }
    virtual QSGRenderLoop *createWindowManager() override { return 0; }
};
