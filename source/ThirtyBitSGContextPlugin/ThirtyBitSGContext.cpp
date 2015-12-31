#include <QtQuick/private/qsgcontext_p.h>
#include <QtQuick/private/qsgdefaultlayer_p.h>
#include "ThirtyBitSGContext.h"
#include "ThirtyBitSGLayer.h"

ThirtyBitSGContext::ThirtyBitSGContext(QObject* parent)
  : QSGContext(parent)
{
}

QSGLayer* ThirtyBitSGContext::createLayer(QSGRenderContext* renderContext)
{
    return new ThirtyBitSGLayer(renderContext);
}
