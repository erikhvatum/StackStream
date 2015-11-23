#include "common.h"
#include "Layer.h"

class LayerRenderer
  : public QQuickFramebufferObject::Renderer,
    protected QOpenGLFunctions
{
public:
    LayerRenderer()
    {
        initializeOpenGLFunctions();
    }

    void render() override
    {
        glClearColor(0, 0, 0, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        update();
    }

    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    void synchronize(QQuickFramebufferObject *item) override
    {
        Layer* layer{static_cast<Layer*>(item)};
        m_layer = *layer;
    }

protected:
    Layer m_layer;
};

Layer::Layer(QQuickItem* parent)
  : QQuickFramebufferObject(parent)
{
}

QQuickFramebufferObject::Renderer* Layer::createRenderer() const
{
    return new LayerRenderer();
}
