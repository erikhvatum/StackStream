#include "ThirtyBitSGContextPlugin.h"


ThirtyBitSGContextPlugin::ThirtyBitSGContextPlugin(QObject *parent) :
    QGenericPlugin(parent)
{
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ThirtyBitSGContextPlugin, ThirtyBitSGContextPlugin)
#endif // QT_VERSION < 0x050000
