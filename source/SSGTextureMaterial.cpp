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

#include "SSGTextureMaterial.h"
#include <QtQuick/private/qsgmaterialshader_p.h>
#include <QtQuick/private/qsgtexture_p.h>
#include <QtGui/qopenglshaderprogram.h>
#include <QtGui/qopenglfunctions.h>

inline static bool isPowerOfTwo(int x)
{
    // Assumption: x >= 1
    return x == (x & -x);
}

QSGMaterialType SSGOpaqueTextureMaterialShader::type;

SSGOpaqueTextureMaterialShader::SSGOpaqueTextureMaterialShader()
    : QSGMaterialShader()
{
    setShaderSourceFile(QOpenGLShader::Vertex, QStringLiteral(":/scenegraph/shaders/opaquetexture.vert"));
    setShaderSourceFile(QOpenGLShader::Fragment, QStringLiteral(":/scenegraph/shaders/texture.frag"));
}

char const *const *SSGOpaqueTextureMaterialShader::attributeNames() const
{
    static char const *const attr[] = { "qt_VertexPosition", "qt_VertexTexCoord", "opacity", 0 };
    return attr;
}

void SSGOpaqueTextureMaterialShader::initialize()
{
    m_matrix_id = program()->uniformLocation("qt_Matrix");
}

void SSGOpaqueTextureMaterialShader::updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect)
{
    Q_ASSERT(oldEffect == 0 || newEffect->type() == oldEffect->type());
    SSGOpaqueTextureMaterial *tx = static_cast<SSGOpaqueTextureMaterial *>(newEffect);
    SSGOpaqueTextureMaterial *oldTx = static_cast<SSGOpaqueTextureMaterial *>(oldEffect);

    if (state.isOpacityDirty())
        program()->setUniformValue(m_opacity_id, state.opacity());

    SSGTexture *t = tx->texture();

#ifndef QT_NO_DEBUG
    if (!ssg_safeguard_texture(t))
        return;
#endif

    t->setMinFiltering(tx->minFiltering());
    t->setMagFiltering(tx->magFiltering());

    t->setHorizontalWrapMode(tx->horizontalWrapMode());
    t->setVerticalWrapMode(tx->verticalWrapMode());
    bool npotSupported = const_cast<QOpenGLContext *>(state.context())
        ->functions()->hasOpenGLFeature(QOpenGLFunctions::NPOTTextureRepeat);
    if (!npotSupported) {
        QSize size = t->textureSize();
        const bool isNpot = !isPowerOfTwo(size.width()) || !isPowerOfTwo(size.height());
        if (isNpot) {
            t->setHorizontalWrapMode(SSGTexture::ClampToEdge);
            t->setVerticalWrapMode(SSGTexture::ClampToEdge);
        }
    }

    t->setMinMipmapFiltering(tx->minMipmapFiltering());
    t->setMagMipmapFiltering(tx->magMipmapFiltering());

    if (oldTx == 0 || oldTx->texture()->textureId() != t->textureId())
        t->bind();
    else
        t->updateBindOptions();

    if (state.isMatrixDirty())
        program()->setUniformValue(m_matrix_id, state.combinedMatrix());
}

SSGOpaqueTextureMaterial::SSGOpaqueTextureMaterial()
  : m_texture(0),
    m_min_filtering(SSGTexture::Nearest),
    m_mag_filtering(SSGTexture::Nearest),
    m_min_mipmap_filtering(SSGTexture::None),
    m_mag_mipmap_filtering(SSGTexture::None),
    m_horizontal_wrap(SSGTexture::ClampToEdge),
    m_vertical_wrap(SSGTexture::ClampToEdge)
{
}

QSGMaterialType *SSGOpaqueTextureMaterial::type() const
{
    return &SSGOpaqueTextureMaterialShader::type;
}

QSGMaterialShader *SSGOpaqueTextureMaterial::createShader() const
{
    return new SSGOpaqueTextureMaterialShader;
}

void SSGOpaqueTextureMaterial::setTexture(SSGTexture *texture)
{
    m_texture = texture;
    setFlag(Blending, m_texture ? m_texture->hasAlphaChannel() : false);
}

int SSGOpaqueTextureMaterial::compare(const QSGMaterial *o) const
{
    Q_ASSERT(o && type() == o->type());
    const SSGOpaqueTextureMaterial *other = static_cast<const SSGOpaqueTextureMaterial *>(o);
    int diff = m_texture->textureId() - other->texture()->textureId();
    if (diff)
        return diff;
    diff = int(m_min_filtering) - int(other->m_min_filtering);
    if (diff)
        return diff;
    return int(m_mag_filtering) - int(other->m_mag_filtering);
}
