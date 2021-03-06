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
#include "SSGTexture.h"

class SSGTextureMaterial
    : public QSGMaterial
{
public:
    SSGTextureMaterial();

    virtual QSGMaterialType *type() const;
    virtual QSGMaterialShader *createShader() const;
    virtual int compare(const QSGMaterial *other) const;

    void setTexture(SSGTexture *texture);
    SSGTexture *texture() const { return m_texture; }

    void setMinFiltering(SSGTexture::Filtering filteringType) { m_min_filtering = filteringType; }
    SSGTexture::Filtering minFiltering() const { return SSGTexture::Filtering(m_min_filtering); }

    void setMagFiltering(SSGTexture::Filtering filteringType) { m_mag_filtering = filteringType; }
    SSGTexture::Filtering magFiltering() const { return SSGTexture::Filtering(m_mag_filtering); }

    void setMinMipmapFiltering(SSGTexture::Filtering filteringType) { m_min_mipmap_filtering = filteringType; }
    SSGTexture::Filtering minMipmapFiltering() const { return SSGTexture::Filtering(m_min_mipmap_filtering); }

    void setMagMipmapFiltering(SSGTexture::Filtering filteringType) { m_mag_mipmap_filtering = filteringType; }
    SSGTexture::Filtering magMipmapFiltering() const { return SSGTexture::Filtering(m_mag_mipmap_filtering); }

    void setHorizontalWrapMode(SSGTexture::WrapMode mode) { m_horizontal_wrap = mode; }
    SSGTexture::WrapMode horizontalWrapMode() const { return SSGTexture::WrapMode(m_horizontal_wrap); }

    void setVerticalWrapMode(SSGTexture::WrapMode mode) { m_vertical_wrap = mode; }
    SSGTexture::WrapMode verticalWrapMode() const { return SSGTexture::WrapMode(m_vertical_wrap); }

protected:
    SSGTexture *m_texture;

    uint m_min_filtering: 2;
    uint m_mag_filtering: 2;
    uint m_min_mipmap_filtering: 2;
    uint m_mag_mipmap_filtering: 2;
    uint m_horizontal_wrap : 1;
    uint m_vertical_wrap: 1;

    uint m_reserved : 26;
};

