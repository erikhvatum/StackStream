#pragma once
#include "common.h"
#include "SSGTexture.h"

// class SSGOpaqueTextureMaterial
//     : public QSGMaterial
// {
// public:
//     SSGOpaqueTextureMaterial();
// 
//     virtual QSGMaterialType *type() const;
//     virtual QSGMaterialShader *createShader() const;
//     virtual int compare(const QSGMaterial *other) const;
// 
//     void setTexture(QSGTexture *texture);
//     QSGTexture *texture() const { return m_texture; }
// 
//     void setMipmapFiltering(QSGTexture::Filtering filteringType) { m_mipmap_filtering = filteringType; }
//     QSGTexture::Filtering mipmapFiltering() const { return QSGTexture::Filtering(m_mipmap_filtering); }
// 
//     void setFiltering(QSGTexture::Filtering filteringType) { m_filtering = filteringType; }
//     QSGTexture::Filtering filtering() const { return QSGTexture::Filtering(m_filtering); }
// 
//     void setHorizontalWrapMode(QSGTexture::WrapMode mode) { m_horizontal_wrap = mode; }
//     QSGTexture::WrapMode horizontalWrapMode() const { return QSGTexture::WrapMode(m_horizontal_wrap); }
// 
//     void setVerticalWrapMode(QSGTexture::WrapMode mode) { m_vertical_wrap = mode; }
//     QSGTexture::WrapMode verticalWrapMode() const { return QSGTexture::WrapMode(m_vertical_wrap); }
// 
// protected:
//     QSGTexture *m_texture;
// 
//     uint m_filtering: 2;
//     uint m_mipmap_filtering: 2;
//     uint m_horizontal_wrap : 1;
//     uint m_vertical_wrap: 1;
// 
//     uint m_reserved : 26;
// };
// 
// 
// class Q_QUICK_EXPORT QSGTextureMaterial : public QSGOpaqueTextureMaterial
// {
// public:
//     virtual QSGMaterialType *type() const;
//     virtual QSGMaterialShader *createShader() const;
// };
// };
