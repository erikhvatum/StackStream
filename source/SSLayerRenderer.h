#pragma once
#include "common.h"

class SSLayer;

class SSLayerRenderer
  : public QQuickFramebufferObject::Renderer,
    protected QOpenGLFunctions
{
public:
    static const QSize sm_defaultFboSize;

    SSLayerRenderer();
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize&) override;
    void synchronize(QQuickFramebufferObject *item) override;

protected:
    // When m_layer is not valid, m_fboSize is set to sm_defaultFboSize and m_tex.isCreated() is false.
    SSLayer m_layer;
    QSize m_fboSize;
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLTexture* m_tex;
    std::size_t m_texSerial;
    int m_vertCoordLoc, m_texLoc, m_viewportSizeLoc;
    int m_rescaleMinLoc, m_rescaleRangeLoc, m_gammaLoc, m_tintLoc;
    static const QVector<QVector2D> sm_quad;
    struct componentCountFormats {
        QOpenGLTexture::TextureFormat texFormat;
        QOpenGLTexture::PixelFormat srcPixelFormat;
    };
    static const componentCountFormats sm_componentCountFormats[];
    static const QOpenGLTexture::PixelType sm_componentPixelTypes[];
};
