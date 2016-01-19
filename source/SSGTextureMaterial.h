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

class SSGOpaqueTextureMaterial
    : public QSGMaterial
{
public:
    SSGOpaqueTextureMaterial();

    virtual QSGMaterialType *type() const;
    virtual QSGMaterialShader *createShader() const;
    virtual int compare(const QSGMaterial *other) const;

    void setTexture(SSGTexture *texture);
    SSGTexture *texture() const { return m_texture; }

    void setMipmapFiltering(SSGTexture::Filtering filteringType) { m_mipmap_filtering = filteringType; }
    SSGTexture::Filtering mipmapFiltering() const { return SSGTexture::Filtering(m_mipmap_filtering); }

    void setFiltering(SSGTexture::Filtering filteringType) { m_filtering = filteringType; }
    SSGTexture::Filtering filtering() const { return SSGTexture::Filtering(m_filtering); }

    void setHorizontalWrapMode(SSGTexture::WrapMode mode) { m_horizontal_wrap = mode; }
    SSGTexture::WrapMode horizontalWrapMode() const { return SSGTexture::WrapMode(m_horizontal_wrap); }

    void setVerticalWrapMode(SSGTexture::WrapMode mode) { m_vertical_wrap = mode; }
    SSGTexture::WrapMode verticalWrapMode() const { return SSGTexture::WrapMode(m_vertical_wrap); }

protected:
    SSGTexture *m_texture;

    uint m_filtering: 2;
    uint m_mipmap_filtering: 2;
    uint m_horizontal_wrap : 1;
    uint m_vertical_wrap: 1;

    uint m_reserved : 26;
};


class Q_QUICK_EXPORT SSGTextureMaterial : public SSGOpaqueTextureMaterial
{
public:
    virtual QSGMaterialType *type() const;
    virtual QSGMaterialShader *createShader() const;
};

#include <private/qtquickglobal_p.h>

class SSGOpaqueTextureMaterialShader : public QSGMaterialShader
{
public:
    SSGOpaqueTextureMaterialShader();

    virtual void updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect);
    virtual char const *const *attributeNames() const;

    static QSGMaterialType type;

protected:
    virtual void initialize();

    int m_matrix_id;
};

class SSGTextureMaterialShader : public SSGOpaqueTextureMaterialShader
{
public:
    SSGTextureMaterialShader();

    virtual void updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect);
    virtual void initialize();

    static QSGMaterialType type;

protected:
    int m_opacity_id;
};
