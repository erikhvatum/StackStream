#include "common.h"
#include "ThirtyBitImageItem.h"

ThirtyBitImageItem::ThirtyBitImageItem(QQuickItem* parent)
  : QQuickItem(parent),
    m_image(nullptr)
{
    setFlag(QQuickItem::ItemHasContents);
}

Image* ThirtyBitImageItem::image()
{
    return m_image;
}

const Image* ThirtyBitImageItem::image() const
{
    return m_image;
}

void ThirtyBitImageItem::setImage(Image* image)
{
    if(m_image != nullptr)
    {
        while(!m_imageSignalConnections.empty())
        {
            disconnect(m_imageSignalConnections.front());
            m_imageSignalConnections.pop_front();
        }
        if(m_image->parent() == this) m_image->deleteLater();
    }
    m_image = image;
    if(m_image == nullptr)
    {
        setImplicitSize(0, 0);
    }
    else
    {
        m_imageSignalConnections.push_front(connect(m_image, &Image::isValidChanged, this, &ThirtyBitImageItem::update));
        m_imageSignalConnections.push_front(connect(m_image, &Image::serialChanged, this, &ThirtyBitImageItem::update));
        m_imageSignalConnections.push_front(connect(m_image, &Image::sizeChanged, this, &ThirtyBitImageItem::onImageSizeChanged));
        m_imageSignalConnections.push_front(connect(m_image, &Image::imageTypeChanged, this, &ThirtyBitImageItem::update));
        m_imageSignalConnections.push_front(connect(m_image, &Image::channelCountChanged, this, &ThirtyBitImageItem::update));
        setImplicitSize(m_image->size().width(), m_image->size().height());
    }
    update();
}

QSGNode* ThirtyBitImageItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData)
{
    qDebug() << "QSGNode* ThirtyBitImageItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData)";
    QSGSimpleRectNode *n = static_cast<QSGSimpleRectNode *>(oldNode);
    if (!n) {
        n = new QSGSimpleRectNode();
        n->setColor(Qt::red);
    }
    n->setRect(boundingRect());
    return n;
}

void ThirtyBitImageItem::onImageSizeChanged(QSize size)
{
    setImplicitSize(size.width(), size.height());
}
