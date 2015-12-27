#include "common.h"
#include "ThirtyBitSGTexture.h"
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

ThirtyBitSGTexture::ThirtyBitSGTexture()
    : QSGTexture()
    , m_texture_id(0)
    , m_has_alpha(false)
    , m_dirty_texture(false)
    , m_dirty_bind_options(false)
    , m_owns_texture(true)
    , m_mipmaps_generated(false)
    , m_retain_image(false)
{
}


ThirtyBitSGTexture::~ThirtyBitSGTexture()
{
    if (m_texture_id && m_owns_texture && QOpenGLContext::currentContext())
        QOpenGLContext::currentContext()->functions()->glDeleteTextures(1, &m_texture_id);
}

void ThirtyBitSGTexture::setImage(const QImage &image)
{
    m_image = image;
    m_texture_size = image.size();
    m_has_alpha = image.hasAlphaChannel();
    m_dirty_texture = true;
    m_dirty_bind_options = true;
    m_mipmaps_generated = false;
 }

int ThirtyBitSGTexture::textureId() const
{
    if (m_dirty_texture) {
        if (m_image.isNull()) {
            // The actual texture and id will be updated/deleted in a later bind()
            // or ~ThirtyBitSGTexture so just keep it minimal here.
            return 0;
        } else if (m_texture_id == 0){
            // Generate a texture id for use later and return it.
            QOpenGLContext::currentContext()->functions()->glGenTextures(1, &const_cast<ThirtyBitSGTexture *>(this)->m_texture_id);
            return m_texture_id;
        }
    }
    return m_texture_id;
}

void ThirtyBitSGTexture::setTextureId(int id)
{
    if (m_texture_id && m_owns_texture)
        QOpenGLContext::currentContext()->functions()->glDeleteTextures(1, &m_texture_id);

    m_texture_id = id;
    m_dirty_texture = false;
    m_dirty_bind_options = true;
    m_image = QImage();
    m_mipmaps_generated = false;
}

void ThirtyBitSGTexture::bind()
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    QOpenGLFunctions *funcs = context->functions();
    if (!m_dirty_texture) {
        funcs->glBindTexture(GL_TEXTURE_2D, m_texture_id);
        if (mipmapFiltering() != QSGTexture::None && !m_mipmaps_generated) {
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

    funcs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2, m_texture_size.width(), m_texture_size.height(), 0, GL_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV, tmp.constBits());

    if (mipmapFiltering() != QSGTexture::None) {
        funcs->glGenerateMipmap(GL_TEXTURE_2D);
        m_mipmaps_generated = true;
    }

    m_texture_rect = QRectF(0, 0, 1, 1);

    m_dirty_bind_options = false;
    if (!m_retain_image)
        m_image = QImage();
}
