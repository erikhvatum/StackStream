#include "common.h"
#include "Layer.h"
#include "LayerRenderer.h"

Layer::Layer(QQuickItem* parent)
  : QQuickFramebufferObject(parent),
    m_image(nullptr),
    m_imageSerial(0),
    m_min(0),
    m_max(1),
    m_gamma(1)
{
    setTextureFollowsItemSize(false);
    setImage(new Image(this));
}

Layer::Layer(const Layer& rhs, QQuickItem* parent)
  : QQuickFramebufferObject(parent),
    m_image(nullptr),
    m_imageSerial(rhs.m_imageSerial),
    m_min(rhs.m_min),
    m_max(rhs.m_max),
    m_gamma(rhs.m_gamma)
{
    setTextureFollowsItemSize(false);
    setImage(const_cast<Layer&>(rhs).m_image);
}

Layer& Layer::operator = (const Layer& rhs)
{
    setImage(const_cast<Layer&>(rhs).m_image);
    setMin(rhs.m_min);
    setMax(rhs.m_max);
    setGamma(rhs.m_gamma);
    return *this;
}

bool Layer::operator == (const Layer& rhs) const
{
    bool ret{false};
    if ( rhs.m_min == m_min
      && rhs.m_max == m_max
      && rhs.m_gamma == m_gamma )
    {
        if(rhs.m_image == m_image)
            ret = true;
        else if(m_image)
            ret = *rhs.m_image == *m_image;
    }
    return ret;
}

QQuickFramebufferObject::Renderer* Layer::createRenderer() const
{
    return new LayerRenderer();
}

bool Layer::isValid() const
{
    return m_image && m_image->isValid();
}

Image* Layer::image()
{
    return m_image;
}

const Image* Layer::image() const
{
    return m_image;
}

void Layer::setImage(Image* image)
{
    bool wasValid{isValid()};
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
    if(m_image != nullptr)
    {
        m_imageSignalConnections.push_front(connect(m_image, &Image::isValidChanged, this, &Layer::isValidChanged));
        m_imageSignalConnections.push_front(connect(m_image, &Image::isValidChanged, this, &Layer::update));
        m_imageSignalConnections.push_front(connect(m_image, &Image::dataChanged, this, &Layer::onImageDataChanged));
        m_imageSignalConnections.push_front(connect(m_image, &Image::sizeChanged, this, &Layer::update));
        m_imageSignalConnections.push_front(connect(m_image, &Image::imageTypeChanged, this, &Layer::update));
        m_imageSignalConnections.push_front(connect(m_image, &Image::channelCountChanged, this, &Layer::update));
    }
    bool nowValid{isValid()};
    imageChanged();
    if(wasValid != nowValid) isValidChanged(nowValid);
    update();
}

const std::size_t& Layer::imageSerial() const
{
    return m_imageSerial;
}

double Layer::min() const
{
    return m_min;
}

void Layer::setMin(double min)
{
    if(m_min != min)
    {
        m_min = min;
        minChanged(m_min);
    }
}

double Layer::max() const
{
    return m_max;
}

void Layer::setMax(double max)
{
    if(m_max != max)
    {
        m_max = max;
        maxChanged(m_max);
    }
}

double Layer::gamma() const
{
    return m_gamma;
}

void Layer::setGamma(double gamma)
{
    if(m_gamma != gamma)
    {
        m_gamma = gamma;
        gammaChanged(m_gamma);
    }
}

void Layer::aboutQt() const
{
    QApplication::aboutQt();
}

void Layer::onImageDataChanged()
{
    ++m_imageSerial;
    imageSerialChanged(m_imageSerial);
    update();
}