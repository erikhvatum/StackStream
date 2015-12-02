#include "common.h"
#include "Layer.h"
#include "LayerRenderer.h"

const QSize LayerRenderer::sm_defaultFboSize(100, 100);

const LayerRenderer::ChannelCountFormats LayerRenderer::sm_channelCountFormats[] = {
    {QOpenGLTexture::R32F, QOpenGLTexture::Red},
    {QOpenGLTexture::RG32F, QOpenGLTexture::RG},
    {QOpenGLTexture::RGB32F, QOpenGLTexture::RGB},
    {QOpenGLTexture::RGBA32F, QOpenGLTexture::RGBA}
};

const QOpenGLTexture::PixelType LayerRenderer::sm_componentPixelTypes[] = {
    QOpenGLTexture::NoPixelType,
    QOpenGLTexture::UInt8,
    QOpenGLTexture::UInt16,
    QOpenGLTexture::UInt16,
    QOpenGLTexture::UInt32,
    QOpenGLTexture::NoPixelType, // TODO: add support; may require abandoning QOpenGLTexture
    QOpenGLTexture::Float32,
    QOpenGLTexture::NoPixelType
};

const QVector<QVector2D> LayerRenderer::sm_quad{
    {1.1f, -1.1f},
    {-1.1f, -1.1f},
    {-1.1f, 1.1f},
    {1.1f, 1.1f}
};

LayerRenderer::LayerRenderer()
  : m_layerSerial(0),
    m_fboSize(sm_defaultFboSize),
    m_tex(QOpenGLTexture::Target2D),
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
    vShad->compileSourceCode(vShadSrc);

    QOpenGLShader* fShad{new QOpenGLShader(QOpenGLShader::Fragment, &m_shaderProgram)};
    const char* fShadSrc =
        "uniform sampler2D tex;\n"
        "uniform vec2 viewportSize;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    vec2 texCoord = gl_FragCoord.xy / gl_FragCoord.w;\n"
        "    texCoord /= viewportSize;\n"
        "    gl_FragColor = (texCoord.x > 0.5f || texCoord.y > 0.5f) ? vec4(clamp(texture2D(tex, texCoord).r, 0, 1)*65535, 0.1, 0, 1) : vec4(1,0,0,1);\n"
        "}\n";
    fShad->compileSourceCode(fShadSrc);

    m_shaderProgram.addShader(vShad);
    m_shaderProgram.addShader(fShad);
    m_shaderProgram.link();

    m_vertCoordLoc = m_shaderProgram.attributeLocation("vertCoord");
    m_texLoc = m_shaderProgram.uniformLocation("tex");
    m_viewportSizeLoc = m_shaderProgram.uniformLocation("viewportSize");

    m_tex.setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    m_tex.setMipLevels(1);
    m_tex.setAutoMipMapGenerationEnabled(false);
    m_tex.setWrapMode(QOpenGLTexture::ClampToEdge);
}

void LayerRenderer::render()
{
    glClearColor(0, 0, 0, 1);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(m_layer.isValid())
    {
        Image& image = *m_layer.image();
        const ChannelCountFormats& formats = sm_channelCountFormats[image.channelCount()];
        if ( m_tex.isCreated()
          && ( m_tex.width() != image.size().width()
            || m_tex.height() != image.size().height()
            || m_tex.format() != formats.texFormat ) )
        {
            m_tex.destroy();
        }
        if(!m_tex.isCreated())
        {
            m_tex.create();
            m_tex.setFormat(formats.texFormat);
            m_tex.setSize(image.size().width(), image.size().height(), 1);
            m_tex.allocateStorage();
            m_texSerial = std::numeric_limits<std::size_t>::max();
        }
        m_tex.bind(0);
        if(m_texSerial != m_layerSerial)
        {
            QOpenGLPixelTransferOptions ptos;
            ptos.setAlignment(1);
            m_tex.setData(formats.srcPixelFormat,
                          sm_componentPixelTypes[image.componentType()],
                          image.rawData().get(),
                          &ptos);
            m_texSerial = m_layerSerial;
        }
        m_shaderProgram.bind();
        m_shaderProgram.setUniformValue(m_texLoc, 0);
        float viewportRect[4];
        glGetFloatv(GL_VIEWPORT, viewportRect);
        m_shaderProgram.setUniformValue(m_viewportSizeLoc, viewportRect[2], viewportRect[3]);
        m_shaderProgram.enableAttributeArray(m_vertCoordLoc);
        m_shaderProgram.setAttributeArray(m_vertCoordLoc, sm_quad.constData());
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        m_shaderProgram.disableAttributeArray(m_vertCoordLoc);
        m_tex.release(0);
        m_shaderProgram.release();
    }
    update();
}

QOpenGLFramebufferObject* LayerRenderer::createFramebufferObject(const QSize&)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    QOpenGLFramebufferObject* ret{new QOpenGLFramebufferObject(m_fboSize, format)};
    qDebug() << "QOpenGLFramebufferObject* createFramebufferObject(const QSize&) override" << m_fboSize;
    return ret;
}

void LayerRenderer::synchronize(QQuickFramebufferObject *item)
{
    Layer* layer{static_cast<Layer*>(item)};
    QSize desiredSize{m_fboSize};
    if(layer->isValid()) qDebug() << layer->image()->size();
    else qDebug("layer->isValid()");
    if(layer->isValid())
    {
        if(m_layer.isValid() && m_layer.image() != layer->image()) ++m_layerSerial;
        desiredSize = layer->image()->size();
    }
    else if(m_layer.isValid())
    {
        m_tex.destroy();
        desiredSize = sm_defaultFboSize;
    }
    m_layer = *layer;
    qDebug() << "LayerRenderer::synchronize" << m_fboSize << desiredSize;
    if(m_fboSize != desiredSize)
    {
        m_fboSize = desiredSize;
        invalidateFramebufferObject();
    }
}
