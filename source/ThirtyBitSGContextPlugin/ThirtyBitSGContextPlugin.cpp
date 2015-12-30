#include "ThirtyBitSGContextPlugin.h"
#include "ThirtyBitSGContext.h"

ThirtyBitSGContextPlugin::ThirtyBitSGContextPlugin(QObject *parent) 
  : QObject(parent)
{
}

QStringList ThirtyBitSGContextPlugin::keys() const
{
    // This never seems to be invoked.  Perhaps it is vestigial?  It does appear to be
    // redundant, given that the json metadata file contains a "Keys" entry.
    QStringList ret;
    ret << "ThirtyBitSGContextPlugin";
    return ret;
}

QSGContext* ThirtyBitSGContextPlugin::create(const QString& key) const
{
    return new ThirtyBitSGContext();
}
