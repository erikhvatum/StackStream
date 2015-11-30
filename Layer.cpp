#include "common.h"
#include "Layer.h"

class LayerRenderer
  : public QQuickFramebufferObject::Renderer,
    protected QOpenGLFunctions
{
public:
    LayerRenderer()
      : m_layerSerial(0),
        m_texSerial(0)
    {
        initializeOpenGLFunctions();

        QOpenGLShader* vShad{new QOpenGLShader(QOpenGLShader::Vertex, &m_shaderProgram)};
        const char* vShadSrc =
            "attribute highp vec2 vertCoord;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_Position = vec4(vertCoord, 0.5, 1.0);\n"
            "}\n";
        vShad->compilseSourceCode(vShadSrc);

        QOpenGLShader* fShad{new QOpenGLShader(QOpenGLShader::Fragment, &m_shaderProgram)};
        const char* fShadSrc =
            "uniform sampler2D tex;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vec2 texCoord = gl_FragCoord.xy / gl_FragCoord.w;\n"
            "    gl_FragColor = texture2D(tex, texCoord);\n"
            "}\n";
        fShad->compilseSourceCode(fShadSrc);

        m_shaderProgram.addShader(vShad);
        m_shaderProgram.addShader(fShad);
        m_shaderProgram.link();

        m_vertCoordLoc = m_shaderProgram.attributeLocation("vertCoord");
        m_texLoc = m_shaderProgram.uniformLocation("tex");
    }

    void render() override
    {
        glClearColor(0, 0, 0, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(m_layer.isValid())
        {
            if(m_texSerial != m_layerSerial)
            {
            }
            m_shaderProgram.bind();
            m_shaderProgram.setUniformValue(m_texLoc, 0);
            m_shaderProgram.enableAttributeArray(m_vertCoordLoc);
            m_shaderProgram.setAttributeArray(m_vertCoordLoc, sm_quad.constData());
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            m_shaderProgram.disableAttributeArray(m_vertCoordLoc);
        }
//      update();
    }

    QOpenGLFramebufferObject* createFramebufferObject(const QSize&) override
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        QOpenGLFramebufferObject* ret{new QOpenGLFramebufferObject(m_desiredTextureSize, format)};
        m_textureSize = m_desiredTextureSize;
        qDebug() << "QOpenGLFramebufferObject* createFramebufferObject(const QSize&) override" << m_textureSize;
        return ret;
    }

    void synchronize(QQuickFramebufferObject *item) override
    {
        Layer* layer{static_cast<Layer*>(item)};
        if(layer.isValid())
        {
            ++m_layerSerial;
            m_desiredTextureSize = layer.image()->size();
        }
        else if(m_layer.isValid())
        {
            m_tex.destroy();
            m_desiredTextureSize = sm_defaultTextureSize;
        }
        m_layer = *layer;
        if(m_desiredTextureSize != m_textureSize) invalidateFramebufferObject();
    }

protected:
    Layer m_layer;
    std::size_t m_layerSerial;
    QSize m_textureSize, m_desiredTextureSize;
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLTexture m_tex;
    std::size_t m_texSerial;
    int m_vertCoordLoc, m_texLoc;
    static const QSize sm_defaultTextureSize;
    static const QVector<QVector2D> sm_quad;
};

const QSize LayerRenderer::sm_defaultTextureSize(100, 100);

const QVector<QVector2D> LayerRenderer::sm_quad{
    {1.1, -1.1},
    {-1.1, -1.1},
    {-1.1, 1.1},
    {1.1, 1.1}
};

Layer::Layer(QQuickItem* parent)
  : QQuickFramebufferObject(parent),
    m_image(nullptr),
    m_min(0),
    m_max(1),
    m_gamma(1)
{
    m_image = new Image(this);
    setTextureFollowsItemSize(false);
}

Layer::Layer(const Layer& rhs, QQuickItem* parent)
  : QQuickFramebufferObject(parent),
    m_image(nullptr),
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
        m_imageSignalConnections.push_front(connect(m_image, &Image::dataChanged, this, &Layer::update));
        m_imageSignalConnections.push_front(connect(m_image, &Image::sizeChanged, this, &Layer::update));
        m_imageSignalConnections.push_front(connect(m_image, &Image::imageTypeChanged, this, &Layer::update));
        m_imageSignalConnections.push_front(connect(m_image, &Image::channelCountChanged, this, &Layer::update));
    }
    bool nowValid{isValid()};
    if(wasValid != nowValid) isValidChanged(nowValid);
    update();
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

