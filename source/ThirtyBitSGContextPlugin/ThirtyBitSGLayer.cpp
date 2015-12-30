#include <QtQuick/private/qsgdefaultlayer_p.h>
#include <QOpenGLFramebufferObject>
#include "ThirtyBitSGLayer.h"

ThirtyBitSGLayer::ThirtyBitSGLayer(QSGRenderContext* context)
  : QSGDefaultLayer(context)
{
    qDebug() << "ThirtyBitSGLayer::ThirtyBitSGLayer(QSGRenderContext* context)";
    m_format = GL_RGB10_A2;
}

void ThirtyBitSGLayer::bind()
{
    qDebug() << "ThirtyBitSGLayer::bind()";
#ifndef QT_NO_DEBUG
    if (!m_recursive && m_fbo && ((m_multisampling && m_secondaryFbo->isBound()) || m_fbo->isBound()))
        qWarning("ShaderEffectSource: \'recursive\' must be set to true when rendering recursively.");
#endif
    QOpenGLFunctions *funcs = QOpenGLContext::currentContext()->functions();
    if (!m_fbo && m_format == GL_RGBA) {
        if (m_transparentTexture == 0) {
            funcs->glGenTextures(1, &m_transparentTexture);
            funcs->glBindTexture(GL_TEXTURE_2D, m_transparentTexture);
            const uint zero = 0;
            funcs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &zero);
        } else {
            funcs->glBindTexture(GL_TEXTURE_2D, m_transparentTexture);
        }
    } else {
        funcs->glBindTexture(GL_TEXTURE_2D, m_fbo ? m_fbo->texture() : 0);
        updateBindOptions();
    }
}
