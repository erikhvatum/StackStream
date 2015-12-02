#include "common.h"
#include "Layer.h"
#include "LayerRenderer.h"

const QSize LayerRenderer::sm_defaultFboSize(100, 100);

const QVector<QVector2D> LayerRenderer::sm_quad{
    {1.1, -1.1},
    {-1.1, -1.1},
    {-1.1, 1.1},
    {1.1, 1.1}
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
        "\n"
        "void main()\n"
        "{\n"
        "    vec2 texCoord = gl_FragCoord.xy / gl_FragCoord.w;\n"
        "    gl_FragColor = texture2D(tex, texCoord);\n"
        "}\n";
    fShad->compileSourceCode(fShadSrc);

    m_shaderProgram.addShader(vShad);
    m_shaderProgram.addShader(fShad);
    m_shaderProgram.link();

    m_vertCoordLoc = m_shaderProgram.attributeLocation("vertCoord");
    m_texLoc = m_shaderProgram.uniformLocation("tex");
}

void LayerRenderer::render()
{
    glClearColor(0, 0, 0, 1);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(m_layer.isValid())
    {
        if(m_texSerial != m_layerSerial)
        {
            if ( m_tex.isCreated()
              && ( m_tex.width() != m_layer.image()->size().height()
                || m_tex.height() != m_layer.image()->size().height()

                 )
               )
            {
            }
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
    /*Layer* layer{static_cast<Layer*>(item)};
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
    if(m_desiredTextureSize != m_textureSize) invalidateFramebufferObject();*/
}

