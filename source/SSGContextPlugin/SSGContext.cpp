#include <QtQuick/private/qsgcontext_p.h>
#include <QtQuick/private/qsgdefaultlayer_p.h>
#include "SSGContext.h"
#include "SSGLayer.h"

SSGContext::SSGContext(QObject* parent)
  : QSGContext(parent)
{
}

QSGLayer* SSGContext::createLayer(QSGRenderContext* renderContext)
{
    return new SSGLayer(renderContext);
}
