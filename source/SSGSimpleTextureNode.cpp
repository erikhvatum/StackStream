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

#include "SSGSimpleTextureNode.h"
#include <private/qsgnode_p.h>

class SSGSimpleTextureNodePrivate : public QSGGeometryNodePrivate
{
public:
    SSGSimpleTextureNodePrivate()
        : QSGGeometryNodePrivate()
        , texCoordMode(SSGSimpleTextureNode::NoTransform)
        , ownsTexture(false)
    {}

    QRectF sourceRect;
    SSGSimpleTextureNode::TextureCoordinatesTransformMode texCoordMode;
    uint ownsTexture : 1;
};

static void SSGSimpleTextureNode_update(QSGGeometry *g,
                                        SSGTexture *texture,
                                        const QRectF &rect,
                                        QRectF sourceRect,
                                        SSGSimpleTextureNode::TextureCoordinatesTransformMode texCoordMode)
{
    if (!texture)
        return;

    if (!sourceRect.width() || !sourceRect.height()) {
        QSize ts = texture->textureSize();
        sourceRect = QRectF(0, 0, ts.width(), ts.height());
    }

    // Maybe transform the texture coordinates
    if (texCoordMode.testFlag(SSGSimpleTextureNode::MirrorHorizontally)) {
        float tmp = sourceRect.left();
        sourceRect.setLeft(sourceRect.right());
        sourceRect.setRight(tmp);
    }
    if (texCoordMode.testFlag(SSGSimpleTextureNode::MirrorVertically)) {
        float tmp = sourceRect.top();
        sourceRect.setTop(sourceRect.bottom());
        sourceRect.setBottom(tmp);
    }

    QSGGeometry::updateTexturedRectGeometry(g, rect, texture->convertToNormalizedSourceRect(sourceRect));
}

SSGSimpleTextureNode::SSGSimpleTextureNode()
  : QSGGeometryNode(*new SSGSimpleTextureNodePrivate),
    m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4),
    m_ownsTexture(false),
    m_texCoordMode(NoTransform)
{
    setGeometry(&m_geometry);
    setMaterial(&m_material);
    setOpaqueMaterial(&m_opaque_material);
    m_material.setMinMipmapFiltering(SSGTexture::None);
    m_material.setMagMipmapFiltering(SSGTexture::None);
    m_opaque_material.setMinMipmapFiltering(SSGTexture::None);
    m_opaque_material.setMagMipmapFiltering(SSGTexture::None);
#ifdef QSG_RUNTIME_DESCRIPTION
    qsgnode_set_description(this, QLatin1String("SSsimpletexture"));
#endif
}

SSGSimpleTextureNode::~SSGSimpleTextureNode()
{
    if(m_ownsTexture) delete m_material.texture();
}

void SSGSimpleTextureNode::setMinFiltering(SSGTexture::Filtering filtering)
{
    if (m_material.minFiltering() == filtering)
        return;

    m_material.setMinFiltering(filtering);
    m_opaque_material.setMinFiltering(filtering);
    markDirty(DirtyMaterial);
}

SSGTexture::Filtering SSGSimpleTextureNode::minFiltering() const
{
    return m_material.minFiltering();
}

void SSGSimpleTextureNode::setMagFiltering(SSGTexture::Filtering filtering)
{
    if (m_material.magFiltering() == filtering)
        return;

    m_material.setMagFiltering(filtering);
    m_opaque_material.setMagFiltering(filtering);
    markDirty(DirtyMaterial);
}

SSGTexture::Filtering SSGSimpleTextureNode::magFiltering() const
{
    return m_material.magFiltering();
}

void SSGSimpleTextureNode::setRect(const QRectF &r)
{
    if (m_rect == r)
        return;
    m_rect = r;
    Q_D(SSGSimpleTextureNode);
    SSGSimpleTextureNode_update(&m_geometry, texture(), m_rect, d->sourceRect, d->texCoordMode);
    markDirty(DirtyGeometry);
}

QRectF SSGSimpleTextureNode::rect() const
{
    return m_rect;
}

void SSGSimpleTextureNode::setSourceRect(const QRectF &r)
{
    Q_D(SSGSimpleTextureNode);
    if (d->sourceRect == r)
        return;
    d->sourceRect = r;
    SSGSimpleTextureNode_update(&m_geometry, texture(), m_rect, d->sourceRect, d->texCoordMode);
    markDirty(DirtyGeometry);
}

QRectF SSGSimpleTextureNode::sourceRect() const
{
    Q_D(const SSGSimpleTextureNode);
    return d->sourceRect;
}

void SSGSimpleTextureNode::setTexture(SSGTexture *texture)
{
    Q_ASSERT(texture);
    Q_D(SSGSimpleTextureNode);
    if (d->ownsTexture)
        delete m_material.texture();
    m_material.setTexture(texture);
    m_opaque_material.setTexture(texture);
    SSGSimpleTextureNode_update(&m_geometry, texture, m_rect, d->sourceRect, d->texCoordMode);
    markDirty(DirtyMaterial);
}



SSGTexture *SSGSimpleTextureNode::texture() const
{
    return m_material.texture();
}

void SSGSimpleTextureNode::setTextureCoordinatesTransform(SSGSimpleTextureNode::TextureCoordinatesTransformMode mode)
{
    Q_D(SSGSimpleTextureNode);
    if (d->texCoordMode == mode)
        return;
    d->texCoordMode = mode;
    SSGSimpleTextureNode_update(&m_geometry, texture(), m_rect, d->sourceRect, d->texCoordMode);
    markDirty(DirtyMaterial);
}

SSGSimpleTextureNode::TextureCoordinatesTransformMode SSGSimpleTextureNode::textureCoordinatesTransform() const
{
    Q_D(const SSGSimpleTextureNode);
    return d->texCoordMode;
}

void SSGSimpleTextureNode::setOwnsTexture(bool owns)
{
    Q_D(SSGSimpleTextureNode);
    d->ownsTexture = owns;
}

bool SSGSimpleTextureNode::ownsTexture() const
{
    Q_D(const SSGSimpleTextureNode);
    return d->ownsTexture;
}
