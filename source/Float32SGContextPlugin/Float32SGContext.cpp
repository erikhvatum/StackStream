#include <QtQuick/private/qsgcontext_p.h>
#include <QtQuick/private/qsgdefaultlayer_p.h>
#include "Float32SGContext.h"
#include "Float32SGLayer.h"

Float32SGContext::Float32SGContext(QObject* parent)
  : QSGContext(parent)
{
}

QSGLayer* Float32SGContext::createLayer(QSGRenderContext* renderContext)
{
    return new Float32SGLayer(renderContext);
}
