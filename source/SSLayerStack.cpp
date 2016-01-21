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
#include "SSLayerStack.h"

SSLayerStack::SSLayerStack(QQuickItem* parent)
  : QQuickItem(parent),
    m_layer0(nullptr),
    m_layer1(nullptr),
    m_layer2(nullptr),
    m_layer3(nullptr),
    m_layer4(nullptr),
    m_layer5(nullptr),
    m_layer6(nullptr),
    m_layer7(nullptr)
{
}

SSLayerStack::~SSLayerStack() {}

SSLayer* SSLayerStack::layer0() const {return m_layer0;}
SSLayer* SSLayerStack::layer1() const {return m_layer1;}
SSLayer* SSLayerStack::layer2() const {return m_layer2;}
SSLayer* SSLayerStack::layer3() const {return m_layer3;}
SSLayer* SSLayerStack::layer4() const {return m_layer4;}
SSLayer* SSLayerStack::layer5() const {return m_layer5;}
SSLayer* SSLayerStack::layer6() const {return m_layer6;}
SSLayer* SSLayerStack::layer7() const {return m_layer7;}

void SSLayerStack::setLayer0(SSLayer* layer) {
    if(m_layer0 != layer) {
        m_layer0 = layer;
        layer0Changed(m_layer0);
    }
}

void SSLayerStack::setLayer1(SSLayer* layer) {
    if(m_layer1 != layer) {
        m_layer1 = layer;
        layer1Changed(m_layer1);
    }
}

void SSLayerStack::setLayer2(SSLayer* layer) {
    if(m_layer2 != layer) {
        m_layer2 = layer;
        layer2Changed(m_layer2);
    }
}

void SSLayerStack::setLayer3(SSLayer* layer) {
    if(m_layer3 != layer) {
        m_layer3 = layer;
        layer3Changed(m_layer3);
    }
}

void SSLayerStack::setLayer4(SSLayer* layer) {
    if(m_layer4 != layer) {
        m_layer4 = layer;
        layer4Changed(m_layer4);
    }
}

void SSLayerStack::setLayer5(SSLayer* layer) {
    if(m_layer5 != layer) {
        m_layer5 = layer;
        layer5Changed(m_layer5);
    }
}

void SSLayerStack::setLayer6(SSLayer* layer) {
    if(m_layer6 != layer) {
        m_layer6 = layer;
        layer6Changed(m_layer6);
    }
}

void SSLayerStack::setLayer7(SSLayer* layer) {
    if(m_layer7 != layer) {
        m_layer7 = layer;
        layer7Changed(m_layer7);
    }
}

QSGNode* SSLayerStack::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* /*updatePaintNodeData*/)
{
    return nullptr;
}
