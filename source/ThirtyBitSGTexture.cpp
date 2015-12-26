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

void qsg_swizzleBGRAToRGBA(QImage *image)
{
    const int width = image->width();
    const int height = image->height();
    for (int i = 0; i < height; ++i) {
        uint *p = (uint *) image->scanLine(i);
        for (int x = 0; x < width; ++x)
            p[x] = ((p[x] << 16) & 0xff0000) | ((p[x] >> 16) & 0xff) | (p[x] & 0xff00ff00);
    }
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

    // ### TODO: check for out-of-memory situations...

    QImage tmp = (m_image.format() == QImage::Format_RGB32 || m_image.format() == QImage::Format_ARGB32_Premultiplied)
                 ? m_image
                 : m_image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    // Downscale the texture to fit inside the max texture limit if it is too big.
    // It would be better if the image was already downscaled to the right size,
    // but this information is not always available at that time, so as a last
    // resort we can do it here. Texture coordinates are normalized, so it
    // won't cause any problems and actual texture sizes will be written
    // based on QSGTexture::textureSize which is updated after this, so that
    // should be ok.
    int max;
    if (QSGRenderContext *rc = QSGRenderContext::from(context))
        max = rc->maxTextureSize();
    else
        funcs->glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
    if (tmp.width() > max || tmp.height() > max) {
        tmp = tmp.scaled(qMin(max, tmp.width()), qMin(max, tmp.height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        m_texture_size = tmp.size();
    }

    // Scale to a power of two size if mipmapping is requested and the
    // texture is npot and npot textures are not properly supported.
    if (mipmapFiltering() != QSGTexture::None
        && (!isPowerOfTwo(m_texture_size.width()) || !isPowerOfTwo(m_texture_size.height()))
        && !funcs->hasOpenGLFeature(QOpenGLFunctions::NPOTTextures)) {
        tmp = tmp.scaled(qNextPowerOfTwo(m_texture_size.width()), qNextPowerOfTwo(m_texture_size.height()),
                         Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        m_texture_size = tmp.size();
    }

    if (tmp.width() * 4 != tmp.bytesPerLine())
        tmp = tmp.copy();

    updateBindOptions(m_dirty_bind_options);

    GLenum externalFormat = GL_RGBA;
    GLenum internalFormat = GL_RGBA;

#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_NO_SDK)
    QString *deviceName =
            static_cast<QString *>(QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("AndroidDeviceName"));
    static bool wrongfullyReportsBgra8888Support = deviceName != 0
                                                    && (deviceName->compare(QStringLiteral("samsung SM-T211"), Qt::CaseInsensitive) == 0
                                                        || deviceName->compare(QStringLiteral("samsung SM-T210"), Qt::CaseInsensitive) == 0
                                                        || deviceName->compare(QStringLiteral("samsung SM-T215"), Qt::CaseInsensitive) == 0);
#else
    static bool wrongfullyReportsBgra8888Support = false;
#endif

    if (context->hasExtension(QByteArrayLiteral("GL_EXT_bgra"))) {
        externalFormat = GL_BGRA;
#ifdef QT_OPENGL_ES
        internalFormat = GL_BGRA;
#else
        if (context->isOpenGLES())
            internalFormat = GL_BGRA;
#endif // QT_OPENGL_ES
    } else if (!wrongfullyReportsBgra8888Support
               && (context->hasExtension(QByteArrayLiteral("GL_EXT_texture_format_BGRA8888"))
                   || context->hasExtension(QByteArrayLiteral("GL_IMG_texture_format_BGRA8888")))) {
        externalFormat = GL_BGRA;
        internalFormat = GL_BGRA;
#ifdef Q_OS_IOS
    } else if (context->hasExtension(QByteArrayLiteral("GL_APPLE_texture_format_BGRA8888"))) {
        externalFormat = GL_BGRA;
        internalFormat = GL_RGBA;
#endif
    } else {
        qsg_swizzleBGRAToRGBA(&tmp);
    }

    funcs->glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_texture_size.width(), m_texture_size.height(), 0, externalFormat, GL_UNSIGNED_BYTE, tmp.constBits());

    if (mipmapFiltering() != QSGTexture::None) {
        funcs->glGenerateMipmap(GL_TEXTURE_2D);
        m_mipmaps_generated = true;
    }

    m_texture_rect = QRectF(0, 0, 1, 1);

    m_dirty_bind_options = false;
    if (!m_retain_image)
        m_image = QImage();
}
