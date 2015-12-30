#include <QtQuick/private/qsgcontext_p.h>
#include "ThirtyBitSGContext.h"

ThirtyBitSGContext::ThirtyBitSGContext(QObject* parent)
  : QSGContext(parent)
{
    qDebug() << "ThirtyBitSGContext::ThirtyBitSGContext(QObject* parent)";
}
