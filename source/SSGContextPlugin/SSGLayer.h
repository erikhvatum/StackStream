#pragma once

#include <QtQuick/private/qsgadaptationlayer_p.h>
#include <QtQuick/private/qsgcontext_p.h>
#include <qsgsimplerectnode.h>

#define QSG_DEBUG_FBO_OVERLAY

class SSGLayer : public QSGLayer
{
    Q_OBJECT
public:
    SSGLayer(QSGRenderContext *context);
    ~SSGLayer();

    bool updateTexture() Q_DECL_OVERRIDE;

    // The item's "paint node", not effect node.
    QSGNode *item() const { return m_item; }
    void setItem(QSGNode *item) Q_DECL_OVERRIDE;

    QRectF rect() const { return m_rect; }
    void setRect(const QRectF &rect) Q_DECL_OVERRIDE;

    QSize size() const { return m_size; }
    void setSize(const QSize &size) Q_DECL_OVERRIDE;

    void setHasMipmaps(bool mipmap) Q_DECL_OVERRIDE;

    void bind() Q_DECL_OVERRIDE;

    bool hasAlphaChannel() const Q_DECL_OVERRIDE;
    bool hasMipmaps() const Q_DECL_OVERRIDE;
    int textureId() const Q_DECL_OVERRIDE;
    QSize textureSize() const Q_DECL_OVERRIDE { return m_size; }

    GLenum format() const { return m_format; }
    void setFormat(GLenum format) Q_DECL_OVERRIDE;

    bool live() const { return bool(m_live); }
    void setLive(bool live) Q_DECL_OVERRIDE;

    bool recursive() const { return bool(m_recursive); }
    void setRecursive(bool recursive) Q_DECL_OVERRIDE;

    void setDevicePixelRatio(qreal ratio) Q_DECL_OVERRIDE { m_device_pixel_ratio = ratio; }

    bool mirrorHorizontal() const { return bool(m_mirrorHorizontal); }
    void setMirrorHorizontal(bool mirror) Q_DECL_OVERRIDE;

    bool mirrorVertical() const { return bool(m_mirrorVertical); }
    void setMirrorVertical(bool mirror) Q_DECL_OVERRIDE;

    void scheduleUpdate() Q_DECL_OVERRIDE;

    QImage toImage() const Q_DECL_OVERRIDE;

    QRectF normalizedTextureSubRect() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void markDirtyTexture() Q_DECL_OVERRIDE;
    void invalidated() Q_DECL_OVERRIDE;

protected:
    void grab();

    QSGNode *m_item;
    QRectF m_rect;
    QSize m_size;
    qreal m_device_pixel_ratio;
    GLenum m_format;

    QSGRenderer *m_renderer;
    QOpenGLFramebufferObject *m_fbo;
    QOpenGLFramebufferObject *m_secondaryFbo;
    QSharedPointer<QSGDepthStencilBuffer> m_depthStencilBuffer;

    GLuint m_transparentTexture;

#ifdef QSG_DEBUG_FBO_OVERLAY
    QSGSimpleRectNode *m_debugOverlay;
#endif

    QSGRenderContext *m_context;

    uint m_mipmap : 1;
    uint m_live : 1;
    uint m_recursive : 1;
    uint m_dirtyTexture : 1;
    uint m_multisamplingChecked : 1;
    uint m_multisampling : 1;
    uint m_grab : 1;
    uint m_mirrorHorizontal : 1;
    uint m_mirrorVertical : 1;
};
