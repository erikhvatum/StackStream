#pragma once
#include "common.h"

class SSGTexture
  : public QSGTexture
{
    Q_OBJECT
public:
    SSGTexture();
    virtual ~SSGTexture();

//  void setChannelCount(std::size_t channel_count);
//  const std::size_t& channelCount() const;


    void setOwnsTexture(bool owns) { m_owns_texture = owns; }
    bool ownsTexture() const { return m_owns_texture; }

    void setTextureId(int id);
    int textureId() const;
    void setTextureSize(const QSize &size) { m_texture_size = size; }
    QSize textureSize() const { return m_texture_size; }

    void setHasAlphaChannel(bool alpha) { m_has_alpha = alpha; }
    bool hasAlphaChannel() const { return m_has_alpha; }

    bool hasMipmaps() const { return mipmapFiltering() != QSGTexture::None; }

    void setImage(const QImage &image);
    const QImage &image() { return m_image; }

    virtual void bind();

    static SSGTexture *fromImage(const QImage &image) {
        SSGTexture *t = new SSGTexture();
        t->setImage(image);
        return t;
    }

protected:
    QImage m_image;


    GLuint m_texture_id;
    QSize m_texture_size;
    QRectF m_texture_rect;

    uint m_has_alpha : 1;
    uint m_dirty_texture : 1;
    uint m_dirty_bind_options : 1;
    uint m_owns_texture : 1;
    uint m_mipmaps_generated : 1;
    uint m_retain_image: 1;
};
