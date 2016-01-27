// The MIT License (MIT)
//
// Copyright (c) 2016 Erik Hvatum
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Authors: Erik Hvatum <ice.rikh@gmail.com>

#include "StackStream.h"

const QSize SSLayerRenderer::sm_defaultFboSize(100, 100);

const SSLayerRenderer::componentCountFormats SSLayerRenderer::sm_componentCountFormats[] = {
    {QOpenGLTexture::NoFormat, QOpenGLTexture::NoSourceFormat},
    {QOpenGLTexture::R32F, QOpenGLTexture::Red},
    {QOpenGLTexture::RG32F, QOpenGLTexture::RG},
    {QOpenGLTexture::RGB32F, QOpenGLTexture::RGB},
    {QOpenGLTexture::RGBA32F, QOpenGLTexture::RGBA}
};

const QOpenGLTexture::PixelType SSLayerRenderer::sm_componentPixelTypes[] = {
    QOpenGLTexture::NoPixelType,
    QOpenGLTexture::UInt8,
    QOpenGLTexture::UInt16,
    QOpenGLTexture::UInt16,
    QOpenGLTexture::UInt32,
    QOpenGLTexture::NoPixelType, // TODO: add support; may require abandoning QOpenGLTexture
    QOpenGLTexture::Float32,
    QOpenGLTexture::NoPixelType  // TODO: add support; may require abandoning QOpenGLTexture
};

const QVector<QVector2D> SSLayerRenderer::sm_quad{
    QVector2D{1.1f, -1.1f},
    QVector2D{-1.1f, -1.1f},
    QVector2D{-1.1f, 1.1f},
    QVector2D{1.1f, 1.1f}
};

SSLayerRenderer::SSLayerRenderer()
  : m_fboSize(sm_defaultFboSize),
    m_tex(QOpenGLTexture::Target2D),
    m_texSerial(0)
{
//    const_cast<QVector<QVector2D>&>(sm_quad) << QVector2D{1.1f, -1.1f};
//    const_cast<QVector<QVector2D>&>(sm_quad) << QVector2D{-1.1f, -1.1f};
//    const_cast<QVector<QVector2D>&>(sm_quad) << QVector2D{-1.1f, 1.1f};
//    const_cast<QVector<QVector2D>&>(sm_quad) << QVector2D{1.1f, 1.1f};
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
    fShad->compileSourceFile(":/SSLayerRenderer.frag");

    m_shaderProgram.addShader(vShad);
    m_shaderProgram.addShader(fShad);
    m_shaderProgram.link();

    m_vertCoordLoc    = m_shaderProgram.attributeLocation("vertCoord");
    m_texLoc          = m_shaderProgram.uniformLocation("tex");
    m_viewportSizeLoc = m_shaderProgram.uniformLocation("viewportSize");
    m_rescaleMinLoc   = m_shaderProgram.uniformLocation("rescaleMin");
    m_rescaleRangeLoc = m_shaderProgram.uniformLocation("rescaleRange");
    m_gammaLoc        = m_shaderProgram.uniformLocation("gamma");
    m_tintLoc         = m_shaderProgram.uniformLocation("tint");

    //m_tex.setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    //m_tex.setMipLevels(11);
    //m_tex.setAutoMipMapGenerationEnabled(true);
    //m_tex.setWrapMode(QOpenGLTexture::ClampToEdge);
}

void SSLayerRenderer::render()
{
    glClearColor(0, 0, 0, 1);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(m_layer.isValid())
    {
        SSImage& image = *m_layer.image();
        const componentCountFormats& formats = sm_componentCountFormats[image.componentCount()];
        if ( m_tex.isCreated()
          && ( m_tex.width() != image.size().width()
            || m_tex.height() != image.size().height()
            || m_tex.format() != formats.texFormat ) )
        {
            m_tex.destroy();
        }
        if(!m_tex.isCreated())
        {
            m_tex.setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
            m_tex.setWrapMode(QOpenGLTexture::ClampToEdge);
            m_tex.setAutoMipMapGenerationEnabled(true);
            m_tex.setMipLevels(11);
            m_tex.setFormat(formats.texFormat);
            m_tex.setSize(image.size().width(), image.size().height(), 1);
            m_tex.allocateStorage();
            m_texSerial = std::numeric_limits<std::size_t>::max();
        }
        if(m_texSerial != m_layer.imageSerial())
        {
            QOpenGLPixelTransferOptions ptos;
            ptos.setAlignment(1);
            m_tex.setData(formats.srcPixelFormat,
                           sm_componentPixelTypes[image.componentDType()],
                           image.rawData().get(),
                           &ptos);
            m_texSerial = m_layer.imageSerial();
        }
        m_tex.bind();
        m_shaderProgram.bind();
        m_shaderProgram.setUniformValue(m_texLoc, 0);
        float viewportRect[4];
        glGetFloatv(GL_VIEWPORT, viewportRect);
        m_shaderProgram.setUniformValue(m_viewportSizeLoc, viewportRect[2], viewportRect[3]);
        m_shaderProgram.enableAttributeArray(m_vertCoordLoc);
        m_shaderProgram.setAttributeArray(m_vertCoordLoc, sm_quad.constData());
        m_shaderProgram.setUniformValue(m_rescaleMinLoc, m_layer.min());
        m_shaderProgram.setUniformValue(m_rescaleRangeLoc, m_layer.max()-m_layer.min());
        m_shaderProgram.setUniformValue(m_gammaLoc, m_layer.gamma());
        const QColor& tint{m_layer.tint()};
        m_shaderProgram.setUniformValue(m_tintLoc, tint.redF(), tint.greenF(), tint.blueF(), tint.alphaF());
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        m_shaderProgram.disableAttributeArray(m_vertCoordLoc);
        m_tex.release(0);
        m_shaderProgram.release();
    }
}

QOpenGLFramebufferObject* SSLayerRenderer::createFramebufferObject(const QSize&)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    // Setting sample count here breaks 10-bit color, causing the FBO to be quantized to 8-bit per channel
    // at some point before being displayed.  TODO: look into what's happening here
//    format.setSamples(4);
    format.setInternalTextureFormat(GL_RGBA32F);
    QOpenGLFramebufferObject* ret{new QOpenGLFramebufferObject(m_fboSize, format)};
    return ret;
}

void SSLayerRenderer::synchronize(SSQuickFramebufferObject *item)
{
    SSLayer* layer{qobject_cast<SSLayer*>(item)};
    QSize desiredSize{layer->isValid() ? layer->image()->size() : sm_defaultFboSize};
    m_layer = *layer;
    if(m_fboSize != desiredSize)
    {
        m_fboSize = desiredSize;
        invalidateFramebufferObject();
    }
}
