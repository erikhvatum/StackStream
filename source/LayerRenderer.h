#pragma once
#include "common.h"

class Layer;

class LayerRenderer
  : public QQuickFramebufferObject::Renderer,
    protected QOpenGLFunctions
{
public:
    LayerRenderer();
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize&) override;
    void synchronize(QQuickFramebufferObject *item) override;

protected:
    // When m_layer is not valid, m_fboSize is set to sm_defaultFboSize and m_tex.isCreated() is false.
    Layer m_layer;
    std::size_t m_layerSerial;
    QSize m_fboSize;
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLTexture m_tex;
    std::size_t m_texSerial;
    int m_vertCoordLoc, m_texLoc;
    static const QSize sm_defaultFboSize;
    static const QVector<QVector2D> sm_quad;
};
