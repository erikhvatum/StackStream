/****************************************************************************
**
** (C) 2016 Erik Hvatum
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "common.h"
#include "SSGTexture.h"
#include "SSLayerStack.h"
#include <QtQuick/QSGTextureProvider>
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGSimpleMaterial>

static const GLenum GL_TEXTURES[] = {
    GL_TEXTURE0,
    GL_TEXTURE1,
    GL_TEXTURE2,
    GL_TEXTURE3,
    GL_TEXTURE4,
    GL_TEXTURE5,
    GL_TEXTURE6,
    GL_TEXTURE7
};

struct LayerStackState {
    std::vector<SSGTexture*> textures;
};

class LayerStackShader
  : public QSGSimpleMaterialShader<LayerStackState>
{
    QSG_DECLARE_SIMPLE_SHADER(LayerStackShader, LayerStackState);

public:
    LayerStackShader()
      : m_fragmentShader("void main() {}\n")
    {
    }

    const char* vertexShader() const
    {
        return
            "attribute highp vec4 aVertex;              \n"
            "attribute highp vec2 aTexCoord;            \n"
            "uniform highp mat4 qt_Matrix;              \n"
            "varying highp vec2 vTexCoord;              \n"
            "void main() {                              \n"
            "    gl_Position = qt_Matrix * aVertex;     \n"
            "    vTexCoord = aTexCoord;                 \n"
            "}";
    }

    const char* fragmentShader() const
    {
        return m_fragmentShader.toUtf8();
    }

    virtual QList<QByteArray> attributes() const
    {
        return QList<QByteArray>() << "aVertex" << "aTexCoord";
    }

    virtual void updateState(const LayerStackState *newState, const LayerStackState *oldState)
    {
        QOpenGLFunctions* gl{QOpenGLContext::currentContext()->functions()};
        m_fragmentShader = "uniform lowp float qt_Opacity;\n";
        for(std::vector<SSGTexture*>::iterator t{newState->textures.begin()}; t != newState->textures.end(); ++t)
        {
            m_fragmentShader+= QString("uniform sampler2D tex%1;\n").arg(t-newState->textures.begin());
        }
        m_fragmentShader+= "void main() {\n";
        m_fragmentShader+= "    vec4 s;\n";
        m_fragmentShader+= "    float da;\n";
        m_fragmentShader+= "    vec3 sca, dca;\n";
        for(std::vector<SSGTexture*>::iterator t{newState->textures.begin()}; t != newState->textures.end(); ++t)
        {
            m_fragmentShader+= QString("    s = texture2D(tex%1, vTexCoord);\n").arg(t-newState->textures.begin());
            if(t - newState->textures.begin() == 0)
            {
                m_fragmentShader+=
                "    sca = s.rgb * s.a;\n"
                "    da = s.a;\n";
            }
            else
            {
                m_fragmentShader+=
                "    sca = s.rgb * s.a;\n"
                "    dca = clamp(sca + dca - sca * dca, 0.0, 1.0);\n";
                "    da = clamp(s.a + da - s.a * da, 0.0, 1.0);\n";
            }
        }
        m_fragmentShader+= "    gl_FragColor = vec4(dca / da, da * qt_Opacity;\n"
                           "}";
        m_textures = newState->textures;
        compile();
    }

    /*void resolveUniforms() {
        // The texture units never change, only the texturess we bind to them so
        // we set these once and for all here. 
        for(int idx = )
        for(std::vector<SSGTexture*>::iterator t{newState.begin()}; t != newState.end(); ++t)
        {
            m_fragmentShader+= QString("uniform sampler2D tex%1;\n").arg(t-newState.begin());
        } 
        program()->setUniformValue("uSource1", 0); // GL_TEXTURE0
        program()->setUniformValue("uSource2", 1); // GL_TEXTURE1
    }*/

    /*virtual char const *const * attributeNames() const
    {
        return nullptr;
    }*/

protected:
    QString m_fragmentShader;
    std::vector<SSGTexture*> m_textures;
};

SSLayerStack::SSLayerStack(QQuickItem* parent)
  : QQuickItem(parent)
{
    m_layers[0] = nullptr;
    m_layers[1] = nullptr;
    m_layers[2] = nullptr;
    m_layers[3] = nullptr;
    m_layers[4] = nullptr;
    m_layers[5] = nullptr;
    m_layers[6] = nullptr;
    m_layers[7] = nullptr;
    m_layersChanged[0] = false;
    m_layersChanged[1] = false;
    m_layersChanged[2] = false;
    m_layersChanged[3] = false;
    m_layersChanged[4] = false;
    m_layersChanged[5] = false;
    m_layersChanged[6] = false;
    m_layersChanged[7] = false;
}

SSLayerStack::~SSLayerStack() {}

SSLayer* SSLayerStack::layer(int idx) const
{
    if(idx >= 0 && idx < 8)
    {
        return m_layers[idx];
    }
    else
    {
        qWarning("SSLayer* SSLayerStack::layer(int idx): idx is not in the interval [0, 7].");
        return nullptr;
    }
}

void SSLayerStack::setLayer(int idx, SSLayer* layer)
{
    if(idx >= 0 && idx < 8)
    {
        if(m_layers[idx] != layer)
        {
            m_layers[idx] = layer;
            m_layersChanged[idx] = true;
            switch(idx)
            {
            case 0:
                layer0Changed(idx, m_layers[idx]);
                break;
            case 1:
                layer1Changed(idx, m_layers[idx]);
                break;
            case 2:
                layer2Changed(idx, m_layers[idx]);
                break;
            case 3:
                layer3Changed(idx, m_layers[idx]);
                break;
            case 4:
                layer4Changed(idx, m_layers[idx]);
                break;
            case 5:
                layer5Changed(idx, m_layers[idx]);
                break;
            case 6:
                layer6Changed(idx, m_layers[idx]);
                break;
            case 7:
                layer7Changed(idx, m_layers[idx]);
                break;
            }
        }
    }
    else
    {
        qWarning("void SSLayerStack::setLayer(int idx, SSLayer* layer): idx is not in the interval [0, 7].");
    }
    m_visibleLayers.clear();
    for(SSLayer *layer{m_layers}, *const layersEnd{m_layers + sizeof(m_layers) / sizeof(SSLayer*)}; layer != layersEnd; ++layer)
        if(layer && layer->isValid() && layer->isVisible())
            m_visibleLayers.push_back(layer);
    update();
}

QSGNode* SSLayerStack::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* /*updatePaintNodeData*/)
{
    return nullptr;
}
