#include "common.h"
#include "SSLayer.h"
#include "SSLayerRenderer.h"

SSLayer::SSLayer(QQuickItem* parent)
  : QQuickFramebufferObject(parent),
    m_image(nullptr),
    m_min(0),
    m_max(1),
    m_gamma(1),
    m_tint(Qt::white)
{
    setTextureFollowsItemSize(false);
}

SSLayer::SSLayer(const SSLayer& rhs, QQuickItem* parent)
  : QQuickFramebufferObject(parent),
    m_image(nullptr),
    m_min(rhs.m_min),
    m_max(rhs.m_max),
    m_gamma(rhs.m_gamma),
    m_tint(rhs.m_tint)
{
    setTextureFollowsItemSize(false);
    setImage(const_cast<SSLayer&>(rhs).m_image);
}

SSLayer::~SSLayer()
{
}

SSLayer& SSLayer::operator = (const SSLayer& rhs)
{
    setImage(const_cast<SSLayer&>(rhs).m_image);
    setMin(rhs.m_min);
    setMax(rhs.m_max);
    setGamma(rhs.m_gamma);
    setTint(rhs.m_tint);
    return *this;
}

bool SSLayer::operator == (const SSLayer& rhs) const
{
    bool ret{false};
    if ( rhs.m_min == m_min
      && rhs.m_max == m_max
      && rhs.m_gamma == m_gamma
      && rhs.m_tint == m_tint )
    {
        if(rhs.m_image == m_image)
            ret = true;
        else if(m_image)
            ret = *rhs.m_image == *m_image;
    }
    return ret;
}

bool SSLayer::operator != (const SSLayer& rhs) const
{
    return !(*this == rhs);
}

QQuickFramebufferObject::Renderer* SSLayer::createRenderer() const
{
    return new SSLayerRenderer();
}

bool SSLayer::isValid() const
{
    return m_image && m_image->isValid();
}

SSImage* SSLayer::image()
{
    return m_image;
}

const SSImage* SSLayer::image() const
{
    return m_image;
}

void SSLayer::setImage(SSImage* image)
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
    if(m_image == nullptr)
    {
        setImplicitSize(SSLayerRenderer::sm_defaultFboSize.width(),
                        SSLayerRenderer::sm_defaultFboSize.height());
    }
    else
    {
        m_imageSignalConnections.push_front(connect(m_image, &SSImage::isValidChanged, this, &SSLayer::isValidChanged));
        m_imageSignalConnections.push_front(connect(m_image, &SSImage::isValidChanged, this, &SSLayer::update));
        m_imageSignalConnections.push_front(connect(m_image, &SSImage::serialChanged, this, &SSLayer::onSerialChanged));
        m_imageSignalConnections.push_front(connect(m_image, &SSImage::sizeChanged, this, &SSLayer::onImageSizeChanged));
        m_imageSignalConnections.push_front(connect(m_image, &SSImage::componentDTypeChanged, this, &SSLayer::update));
        m_imageSignalConnections.push_front(connect(m_image, &SSImage::componentCountChanged, this, &SSLayer::update));
        setImplicitSize(m_image->size().width(), m_image->size().height());
    }
    imageSerialChanged(imageSerial());
    bool nowValid{isValid()};
    if(wasValid != nowValid) isValidChanged(nowValid);
    update();
}

std::size_t SSLayer::imageSerial() const
{
    return m_image ? m_image->serial() : std::numeric_limits<std::size_t>::max();
}

float SSLayer::min() const
{
    return m_min;
}

void SSLayer::setMin(float min)
{
    if(m_min != min)
    {
        m_min = min;
        minChanged(m_min);
        if(m_min > m_max)
            setMax(m_min);
        else if(isValid())
            update();
    }
}

void SSLayer::resetMin()
{
    setMin(0.0f);
}

float SSLayer::max() const
{
    return m_max;
}

void SSLayer::setMax(float max)
{
    if(m_max != max)
    {
        m_max = max;
        maxChanged(m_max);
        if(m_max < m_min)
            setMin(m_max);
        else if(isValid())
            update();
    }
}

void SSLayer::resetMax()
{
    setMax(1.0f);
}

float SSLayer::gamma() const
{
    return m_gamma;
}

void SSLayer::setGamma(float gamma)
{
    if(m_gamma != gamma)
    {
        m_gamma = gamma;
        gammaChanged(m_gamma);
        if(isValid()) update();
    }
}

void SSLayer::resetGamma()
{
    setGamma(1.0f);
}

const QColor& SSLayer::tint() const
{
    return m_tint;
}

void SSLayer::setTint(const QColor& tint)
{
    if(tint != m_tint)
    {
        m_tint = tint;
        tintChanged(m_tint);
        if(isValid()) update();
    }
}

void SSLayer::resetTint()
{
    setTint(QColor(Qt::white));
}

QSGTextureProvider* SSLayer::textureProvider() const
{
    return QQuickFramebufferObject::textureProvider();
}

void SSLayer::aboutQt() const
{
    QApplication::aboutQt();
}

void SSLayer::onSerialChanged(std::size_t serial)
{
    if(m_image == nullptr || !m_image->isValid())
        setImplicitSize(SSLayerRenderer::sm_defaultFboSize.width(),
                        SSLayerRenderer::sm_defaultFboSize.height());
    else
        setImplicitSize(m_image->size().width(), m_image->size().height());

    imageSerialChanged(serial);
    update();
}

void SSLayer::onImageSizeChanged(QSize size)
{
    setImplicitSize(size.width(), size.height());
}
