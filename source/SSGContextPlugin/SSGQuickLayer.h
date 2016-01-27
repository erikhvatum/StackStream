/****************************************************************************
**
** Portions (C) 2015 The Qt Company Ltd.
** Portions (C) 2016 Erik Hvatum
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQuick module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

// NB: SSGQuickLayer is *not* the scene graph personality of an SSLayer.  SSGQuickLayer replaces QSGDefaultLayer,
// the thing that is created when a QML Item's .layer property is set to true.

#pragma once
#include <QtQuick/private/qsgadaptationlayer_p.h>
#include <QtQuick/private/qsgcontext_p.h>
#include <qsgsimplerectnode.h>

#define QSG_DEBUG_FBO_OVERLAY

class SSGQuickLayer : public QSGLayer
{
    Q_OBJECT
public:
    SSGQuickLayer(QSGRenderContext *context);
    ~SSGQuickLayer();

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
