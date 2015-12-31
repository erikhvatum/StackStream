#include "Float32SGContextPlugin.h"
#include "Float32SGContext.h"

Float32SGContextPlugin::Float32SGContextPlugin(QObject *parent) 
  : QObject(parent)
{
}

QStringList Float32SGContextPlugin::keys() const
{
    // This never seems to be invoked.  Perhaps it is vestigial?  It does appear to be
    // redundant, given that the json metadata file contains a "Keys" entry.
    QStringList ret;
    ret << "Float32SGContextPlugin";
    return ret;
}

QSGContext* Float32SGContextPlugin::create(const QString& key) const
{
    return (key == "Float32SGContextPlugin") ? new Float32SGContext() : nullptr;
}
