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

#include "StackStream.h"
#include <QtQml/private/qqmlglobal_p.h>
#include <QtQuick/private/qquickprofiler_p.h>
#include <QtQuick/private/qsgcontext_p.h>
#include <QtQuick/private/qsgmaterialshader_p.h>
#include <QtQuick/private/qsgtexture_p.h>

inline static bool isPowerOfTwo(int x)
{
    // Assumption: x >= 1
    return x == (x & -x);
}

SSGTexture::SSGTexture()
  : m_wrapChanged(false),
    m_filteringChanged(false),
    m_horizontalWrap(SSGTexture::ClampToEdge),
    m_verticalWrap(SSGTexture::ClampToEdge),
    m_minMipmapMode(SSGTexture::None),
    m_magMipmapMode(SSGTexture::None),
    m_minFilterMode(SSGTexture::Nearest),
    m_magFilterMode(SSGTexture::Nearest),
    m_texture_id(0),
    m_has_alpha(false),
    m_dirty_texture(false),
    m_dirty_bind_options(false),
    m_owns_texture(true),
    m_mipmaps_generated(false),
    m_retain_image(false)
{
}

SSGTexture::~SSGTexture()
{
    if (m_texture_id && m_owns_texture && QOpenGLContext::currentContext())
        QOpenGLContext::currentContext()->functions()->glDeleteTextures(1, &m_texture_id);
}

SSGTexture *SSGTexture::removedFromAtlas() const
{
    Q_ASSERT_X(!isAtlasTexture(), "SSGTexture::removedFromAtlas()", "Called on a non-atlas texture");
    return 0;
}

QRectF SSGTexture::normalizedTextureSubRect() const
{
    return QRectF(0, 0, 1, 1);
}

bool SSGTexture::isAtlasTexture() const
{
    return false;
}

void SSGTexture::setImage(const QImage &image)
{
    qDebug() << "SSGTexture::setImage(const QImage &image)";
    m_image = image;
    m_texture_size = image.size();
    m_has_alpha = image.hasAlphaChannel();
    m_dirty_texture = true;
    m_dirty_bind_options = true;
    m_mipmaps_generated = false;
}

int SSGTexture::textureId() const
{
    if (m_dirty_texture) {
        if (m_image.isNull()) {
            // The actual texture and id will be updated/deleted in a later bind()
            // or ~SSGTexture so just keep it minimal here.
            return 0;
        } else if (m_texture_id == 0){
            // Generate a texture id for use later and return it.
            QOpenGLContext::currentContext()->functions()->glGenTextures(1, &const_cast<SSGTexture *>(this)->m_texture_id);
            return m_texture_id;
        }
    }
    return m_texture_id;
}

void SSGTexture::setTextureId(int id)
{
    if (m_texture_id && m_owns_texture)
        QOpenGLContext::currentContext()->functions()->glDeleteTextures(1, &m_texture_id);

    m_texture_id = id;
    m_dirty_texture = false;
    m_dirty_bind_options = true;
    m_image = QImage();
    m_mipmaps_generated = false;
}

void SSGTexture::bind()
{
    qWarning("SSGTexture::bind() called; resampling to 10-bpc");
    QOpenGLContext *context = QOpenGLContext::currentContext();
    QOpenGLFunctions *funcs = context->functions();
    if (!m_dirty_texture) {
        funcs->glBindTexture(GL_TEXTURE_2D, m_texture_id);
        if ((minMipmapFiltering() != SSGTexture::None || magMipmapFiltering() != SSGTexture::None) && !m_mipmaps_generated) {
            funcs->glGenerateMipmap(GL_TEXTURE_2D);
            m_mipmaps_generated = true;
        }
        updateBindOptions(m_dirty_bind_options);
        m_dirty_bind_options = false;
        return;
    }

    m_dirty_texture = false;

    if (m_image.isNull()) {
        if (m_texture_id && m_owns_texture) {
            funcs->glDeleteTextures(1, &m_texture_id);
        }
        m_texture_id = 0;
        m_texture_size = QSize();
        m_has_alpha = false;

        return;
    }

    if (m_texture_id == 0)
        funcs->glGenTextures(1, &m_texture_id);
    funcs->glBindTexture(GL_TEXTURE_2D, m_texture_id);

    QImage tmp = (m_image.format() == QImage::Format_RGB30 || m_image.format() == QImage::Format_A2RGB30_Premultiplied)
                ? m_image
                : m_image.convertToFormat(QImage::Format_A2RGB30_Premultiplied);

    if (tmp.width() * 4 != tmp.bytesPerLine())
        tmp = tmp.copy();

    updateBindOptions(m_dirty_bind_options);

    funcs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_texture_size.width(), m_texture_size.height(), 0, GL_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV, tmp.constBits());

    if (minMipmapFiltering() != SSGTexture::None || magMipmapFiltering() != SSGTexture::None) {
        funcs->glGenerateMipmap(GL_TEXTURE_2D);
        m_mipmaps_generated = true;
    }

    m_texture_rect = QRectF(0, 0, 1, 1);

    m_dirty_bind_options = false;
    if (!m_retain_image)
        m_image = QImage();
}

void SSGTexture::updateBindOptions(bool force)
{
    QOpenGLFunctions *funcs = QOpenGLContext::currentContext()->functions();
    force |= isAtlasTexture();

    if (force || m_filteringChanged) {
        bool minLinear{m_minFilterMode == Linear};
        bool magLinear{m_magFilterMode == Linear};
        GLint minFilter = minLinear ? GL_LINEAR : GL_NEAREST;
        GLint magFilter = magLinear ? GL_LINEAR : GL_NEAREST;

        if (hasMipmaps()) {
            if (m_minMipmapMode == Nearest)
                minFilter = minLinear ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
            else if (m_minMipmapMode == Linear)
                minFilter = minLinear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;

            if (m_magMipmapMode == Nearest)
                magFilter = magLinear ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
            else if (m_magMipmapMode == Linear)
                magFilter = magLinear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
        }
        funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        m_filteringChanged = false;
    }

    if (force || m_wrapChanged) {
#ifndef QT_NO_DEBUG
        if (m_horizontalWrap == Repeat || m_verticalWrap == Repeat) {
            bool npotSupported = QOpenGLFunctions(QOpenGLContext::currentContext()).hasOpenGLFeature(QOpenGLFunctions::NPOTTextures);
            QSize size = textureSize();
            bool isNpot = !isPowerOfTwo(size.width()) || !isPowerOfTwo(size.height());
            if (!npotSupported && isNpot)
                qWarning("Scene Graph: This system does not support the REPEAT wrap mode for non-power-of-two textures.");
        }
#endif
        funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_horizontalWrap == Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_verticalWrap == Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        m_wrapChanged = false;
    }
}

void SSGTexture::setMinMipmapFiltering(Filtering filter)
{
    if (m_minMipmapMode != (uint) filter) {
        m_minMipmapMode = filter;
        m_filteringChanged = true;
    }
}

SSGTexture::Filtering SSGTexture::minMipmapFiltering() const
{
    return (SSGTexture::Filtering) m_minMipmapMode;
}

void SSGTexture::setMagMipmapFiltering(Filtering filter)
{
    if (m_magMipmapMode != (uint) filter) {
        m_magMipmapMode = filter;
        m_filteringChanged = true;
    }
}

SSGTexture::Filtering SSGTexture::magMipmapFiltering() const
{
    return (SSGTexture::Filtering) m_magMipmapMode;
}

void SSGTexture::setMinFiltering(SSGTexture::Filtering filter)
{
    if (m_minFilterMode != (uint) filter) {
        m_minFilterMode = filter;
        m_filteringChanged = true;
    }
}

SSGTexture::Filtering SSGTexture::minFiltering() const
{
    return (SSGTexture::Filtering) m_minFilterMode;
}

void SSGTexture::setMagFiltering(SSGTexture::Filtering filter)
{
    if (m_magFilterMode != (uint) filter) {
        m_magFilterMode = filter;
        m_filteringChanged = true;
    }
}

SSGTexture::Filtering SSGTexture::magFiltering() const
{
    return (SSGTexture::Filtering) m_magFilterMode;
}

void SSGTexture::setHorizontalWrapMode(WrapMode hwrap)
{
    if ((uint) hwrap != m_horizontalWrap) {
        m_horizontalWrap = hwrap;
        m_wrapChanged = true;
    }
}

SSGTexture::WrapMode SSGTexture::horizontalWrapMode() const
{
    return (SSGTexture::WrapMode) m_horizontalWrap;
}

void SSGTexture::setVerticalWrapMode(WrapMode vwrap)
{
    if ((uint) vwrap != m_verticalWrap) {
        m_verticalWrap = vwrap;
        m_wrapChanged = true;
    }
}

SSGTexture::WrapMode SSGTexture::verticalWrapMode() const
{
    return (SSGTexture::WrapMode) m_verticalWrap;
}

#ifndef QT_NO_DEBUG
Q_GLOBAL_STATIC(QSet<SSGTexture *>, ssg_valid_texture_set)
Q_GLOBAL_STATIC(QMutex, ssg_valid_texture_mutex)

bool ssg_safeguard_texture(SSGTexture *texture)
{
    QMutexLocker locker(ssg_valid_texture_mutex());
    if (!ssg_valid_texture_set()->contains(texture)) {
        qWarning() << "Invalid texture accessed:" << (void *) texture;
//        qsg_set_material_failure();
        QOpenGLContext::currentContext()->functions()->glBindTexture(GL_TEXTURE_2D, 0);
        return false;
    }
    return true;
}
#endif
