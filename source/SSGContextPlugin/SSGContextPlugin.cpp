#include "SSGContextPlugin.h"
#include "SSGContext.h"

SSGContextPlugin::SSGContextPlugin(QObject *parent) 
  : QObject(parent)
{
}

QStringList SSGContextPlugin::keys() const
{
    // This never seems to be invoked.  Perhaps it is vestigial?  It does appear to be
    // redundant, given that the json metadata file contains a "Keys" entry.
    QStringList ret;
    ret << "SSGContextPlugin";
    return ret;
}

QSGContext* SSGContextPlugin::create(const QString& key) const
{
    return (key == "SSGContextPlugin") ? new SSGContext() : nullptr;
}
