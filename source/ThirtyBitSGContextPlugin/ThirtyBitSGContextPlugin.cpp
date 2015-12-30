#include "ThirtyBitSGContextPlugin.h"
#include "ThirtyBitSGContext.h"

ThirtyBitSGContextPlugin::ThirtyBitSGContextPlugin(QObject *parent) :
    QObject(parent)
{
}

QStringList ThirtyBitSGContextPlugin::keys() const
{
    qDebug() << "QStringList ThirtyBitSGContextPlugin::keys() const";
    QStringList ret;
    ret << "ThirtyBitSGContextPlugin";
    return ret;
}

QSGContext* ThirtyBitSGContextPlugin::create(const QString& key) const
{
    qDebug() << "QSGContext* ThirtyBitSGContextPlugin::create(const QString& key) const: " << key;
    return new ThirtyBitSGContext();
}
