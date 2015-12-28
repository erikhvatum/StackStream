#pragma once
#include "common.h"
#include "Image.h"

class ThirtyBitImageItem
  : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Image* image READ image WRITE setImage)
public:
    ThirtyBitImageItem(QQuickItem* parent=nullptr);

    Image* image();
    const Image* image() const;
    void setImage(Image* image);

signals:

public slots:

protected:
    Image* m_image;
    std::size_t m_texSerial;
    std::forward_list<QMetaObject::Connection> m_imageSignalConnections;

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;
    void onImageSizeChanged(QSize size);
};
