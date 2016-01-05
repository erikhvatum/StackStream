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

    bool updateTexture() override;

    // The item's "paint node", not effect node.
    QSGNode *item() const { return m_item; }
    void setItem(QSGNode *item) override;

    QRectF rect() const { return m_rect; }
    void setRect(const QRectF &rect) override;

    QSize size() const { return m_size; }
    void setSize(const QSize &size) override;

    void setHasMipmaps(bool mipmap) override;

    void bind() override;

    bool hasAlphaChannel() const override;
    bool hasMipmaps() const override;
    int textureId() const override;
    QSize textureSize() const override { return m_size; }

    GLenum format() const { return m_format; }
    void setFormat(GLenum format) override;

    bool live() const { return bool(m_live); }
    void setLive(bool live) override;

    bool recursive() const { return bool(m_recursive); }
    void setRecursive(bool recursive) override;

    void setDevicePixelRatio(qreal ratio) override { m_device_pixel_ratio = ratio; }

    bool mirrorHorizontal() const { return bool(m_mirrorHorizontal); }
    void setMirrorHorizontal(bool mirror) override;

    bool mirrorVertical() const { return bool(m_mirrorVertical); }
    void setMirrorVertical(bool mirror) override;

    void scheduleUpdate() override;

    QImage toImage() const override;

    QRectF normalizedTextureSubRect() const override;

public Q_SLOTS:
    void markDirtyTexture() override;
    void invalidated() override;

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
