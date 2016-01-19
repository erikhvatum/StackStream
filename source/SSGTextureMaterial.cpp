#include "SSGTextureMaterial.h"
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

QT_BEGIN_NAMESPACE

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
    setShaderSourceFile(QOpenGLShader::Fragment, QStringLiteral(":/scenegraph/shaders/opaquetexture.frag"));
}

char const *const *SSGOpaqueTextureMaterialShader::attributeNames() const
{
    static char const *const attr[] = { "qt_VertexPosition", "qt_VertexTexCoord", 0 };
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

    SSGTexture *t = tx->texture();

#ifndef QT_NO_DEBUG
    if (!qsg_safeguard_texture(t))
        return;
#endif

    t->setFiltering(tx->filtering());

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

    t->setMipmapFiltering(tx->mipmapFiltering());

    if (oldTx == 0 || oldTx->texture()->textureId() != t->textureId())
        t->bind();
    else
        t->updateBindOptions();

    if (state.isMatrixDirty())
        program()->setUniformValue(m_matrix_id, state.combinedMatrix());
}


/*!
    \class SSGOpaqueTextureMaterial
    \brief The SSGOpaqueTextureMaterial class provides a convenient way of
    rendering textured geometry in the scene graph.
    \inmodule QtQuick
    \ingroup qtquick-scenegraph-materials

    The opaque textured material will fill every pixel in a geometry with
    the supplied texture. The material does not respect the opacity of the
    SSGMaterialShader::RenderState, so opacity nodes in the parent chain
    of nodes using this material, have no effect.

    The geometry to be rendered with an opaque texture material requires
    vertices in attribute location 0 and texture coordinates in attribute
    location 1. The texture coordinate is a 2-dimensional floating-point
    tuple. The SSGGeometry::defaultAttributes_TexturedPoint2D returns an
    attribute set compatible with this material.

    The texture to be rendered can be set using setTexture(). How the
    texture should be rendered can be specified using setMipmapFiltering(),
    setFiltering(), setHorizontalWrapMode() and setVerticalWrapMode().
    The rendering state is set on the texture instance just before it
    is bound.

    The opaque textured material respects the current matrix and the alpha
    channel of the texture. It will disregard the accumulated opacity in
    the scenegraph.

    A texture material must have a texture set before it is used as
    a material in the scene graph.
 */



/*!
    Creates a new SSGOpaqueTextureMaterial.

    The default mipmap filtering and filtering mode is set to
    SSGTexture::Nearest. The default wrap modes is set to
    \c SSGTexture::ClampToEdge.

 */
SSGOpaqueTextureMaterial::SSGOpaqueTextureMaterial()
    : m_texture(0)
    , m_filtering(SSGTexture::Nearest)
    , m_mipmap_filtering(SSGTexture::None)
    , m_horizontal_wrap(SSGTexture::ClampToEdge)
    , m_vertical_wrap(SSGTexture::ClampToEdge)
{
}


/*!
    \internal
 */
QSGMaterialType *SSGOpaqueTextureMaterial::type() const
{
    return &SSGOpaqueTextureMaterialShader::type;
}

/*!
    \internal
 */
QSGMaterialShader *SSGOpaqueTextureMaterial::createShader() const
{
    return new SSGOpaqueTextureMaterialShader;
}



/*!
    \fn SSGTexture *SSGOpaqueTextureMaterial::texture() const

    Returns this texture material's texture.
 */



/*!
    Sets the texture of this material to \a texture.

    The material does not take ownership of the texture.
 */

void SSGOpaqueTextureMaterial::setTexture(SSGTexture *texture)
{
    m_texture = texture;
    setFlag(Blending, m_texture ? m_texture->hasAlphaChannel() : false);
}



/*!
    \fn void SSGOpaqueTextureMaterial::setMipmapFiltering(SSGTexture::Filtering filtering)

    Sets the mipmap mode to \a filtering.

    The mipmap filtering mode is set on the texture instance just before the
    texture is bound for rendering.

    If the texture does not have mipmapping support, enabling mipmapping has no
    effect.
 */



/*!
    \fn SSGTexture::Filtering SSGOpaqueTextureMaterial::mipmapFiltering() const

    Returns this material's mipmap filtering mode.

    The default mipmap mode is \c SSGTexture::Nearest.
 */



/*!
    \fn void SSGOpaqueTextureMaterial::setFiltering(SSGTexture::Filtering filtering)

    Sets the filtering to \a filtering.

    The filtering mode is set on the texture instance just before the texture
    is bound for rendering.
 */



/*!
    \fn SSGTexture::Filtering SSGOpaqueTextureMaterial::filtering() const

    Returns this material's filtering mode.

    The default filtering is \c SSGTexture::Nearest.
 */



/*!
    \fn void SSGOpaqueTextureMaterial::setHorizontalWrapMode(SSGTexture::WrapMode mode)

    Sets the horizontal wrap mode to \a mode.

    The horizontal wrap mode is set on the texture instance just before the texture
    is bound for rendering.
 */



 /*!
     \fn SSGTexture::WrapMode SSGOpaqueTextureMaterial::horizontalWrapMode() const

     Returns this material's horizontal wrap mode.

     The default horizontal wrap mode is \c SSGTexutre::ClampToEdge.
  */



/*!
    \fn void SSGOpaqueTextureMaterial::setVerticalWrapMode(SSGTexture::WrapMode mode)

    Sets the vertical wrap mode to \a mode.

    The vertical wrap mode is set on the texture instance just before the texture
    is bound for rendering.
 */



 /*!
     \fn SSGTexture::WrapMode SSGOpaqueTextureMaterial::verticalWrapMode() const

     Returns this material's vertical wrap mode.

     The default vertical wrap mode is \c SSGTexutre::ClampToEdge.
  */



/*!
    \internal
 */

int SSGOpaqueTextureMaterial::compare(const QSGMaterial *o) const
{
    Q_ASSERT(o && type() == o->type());
    const SSGOpaqueTextureMaterial *other = static_cast<const SSGOpaqueTextureMaterial *>(o);
    if (int diff = m_texture->textureId() - other->texture()->textureId())
        return diff;
    return int(m_filtering) - int(other->m_filtering);
}



/*!
    \class SSGTextureMaterial
    \brief The QSGTextureMaterial class provides a convenient way of
    rendering textured geometry in the scene graph.
    \inmodule QtQuick
    \ingroup qtquick-scenegraph-materials

    The textured material will fill every pixel in a geometry with
    the supplied texture.

    The geometry to be rendered with a texture material requires
    vertices in attribute location 0 and texture coordinates in attribute
    location 1. The texture coordinate is a 2-dimensional floating-point
    tuple. The QSGGeometry::defaultAttributes_TexturedPoint2D returns an
    attribute set compatible with this material.

    The texture to be rendered can be set using setTexture(). How the
    texture should be rendered can be specified using setMipmapFiltering(),
    setFiltering(), setHorizontalWrapMode() and setVerticalWrapMode().
    The rendering state is set on the texture instance just before it
    is bound.

    The textured material respects the current matrix and the alpha
    channel of the texture. It will also respect the accumulated opacity
    in the scenegraph.

    A texture material must have a texture set before it is used as
    a material in the scene graph.
 */

QSGMaterialType SSGTextureMaterialShader::type;



/*!
    \internal
 */

QSGMaterialType *SSGTextureMaterial::type() const
{
    return &SSGTextureMaterialShader::type;
}



/*!
    \internal
 */

QSGMaterialShader *SSGTextureMaterial::createShader() const
{
    return new SSGTextureMaterialShader;
}

SSGTextureMaterialShader::SSGTextureMaterialShader()
    : SSGOpaqueTextureMaterialShader()
{
    setShaderSourceFile(QOpenGLShader::Fragment, QStringLiteral(":/scenegraph/shaders/texture.frag"));
}

void SSGTextureMaterialShader::updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect)
{
    Q_ASSERT(oldEffect == 0 || newEffect->type() == oldEffect->type());
    if (state.isOpacityDirty())
        program()->setUniformValue(m_opacity_id, state.opacity());

    SSGOpaqueTextureMaterialShader::updateState(state, newEffect, oldEffect);
}

void SSGTextureMaterialShader::initialize()
{
    SSGOpaqueTextureMaterialShader::initialize();
    m_opacity_id = program()->uniformLocation("opacity");
}

QT_END_NAMESPACE
