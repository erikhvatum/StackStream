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

#pragma once
#include "common.h"

class SSGTexture
  : public QSGTexture
{
    Q_OBJECT
public:
    SSGTexture();
    virtual ~SSGTexture();

    void setOwnsTexture(bool owns) { m_owns_texture = owns; }
    bool ownsTexture() const { return m_owns_texture; }

    void setTextureId(int id);
    int textureId() const;
    void setTextureSize(const QSize &size) { m_texture_size = size; }
    QSize textureSize() const { return m_texture_size; }

    bool hasAlphaChannel() const { return m_has_alpha; }

    bool hasMipmaps() const { return mipmapFiltering() != QSGTexture::None; }

    void setImage(const QImage &image);
    const QImage &image() { return m_image; }

    void bind() override;

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
