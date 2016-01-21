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

#pragma once
#include "common.h"
#include "SSLayer.h"

// This will soon operate on a list of layers rather than having 8 individual layer properties;
// for initial proof of concept purposes, we skip that part.

class SSLayerStack
  : public QQuickItem
{
    Q_OBJECT;
    Q_PROPERTY(SSLayer* layer0 READ layer0 WRITE setLayer0 NOTIFY layer0Changed)
    Q_PROPERTY(SSLayer* layer1 READ layer1 WRITE setLayer1 NOTIFY layer1Changed)
    Q_PROPERTY(SSLayer* layer2 READ layer2 WRITE setLayer2 NOTIFY layer2Changed)
    Q_PROPERTY(SSLayer* layer3 READ layer3 WRITE setLayer3 NOTIFY layer3Changed)
    Q_PROPERTY(SSLayer* layer4 READ layer4 WRITE setLayer4 NOTIFY layer4Changed)
    Q_PROPERTY(SSLayer* layer5 READ layer5 WRITE setLayer5 NOTIFY layer5Changed)
    Q_PROPERTY(SSLayer* layer6 READ layer6 WRITE setLayer6 NOTIFY layer6Changed)
    Q_PROPERTY(SSLayer* layer7 READ layer7 WRITE setLayer7 NOTIFY layer7Changed)
public:
    explicit SSLayerStack(QQuickItem* parent=nullptr);
    virtual ~SSLayerStack();

    virtual SSLayer* layer(int idx) const;

    SSLayer* layer0() const { return layer(0); }
    SSLayer* layer1() const { return layer(1); }
    SSLayer* layer2() const { return layer(2); }
    SSLayer* layer3() const { return layer(3); }
    SSLayer* layer4() const { return layer(4); }
    SSLayer* layer5() const { return layer(5); }
    SSLayer* layer6() const { return layer(6); }
    SSLayer* layer7() const { return layer(7); }

    virtual void setLayer(int idx, SSLayer* layer);

    void setLayer0(SSLayer* layer) { setLayer(0, layer); }
    void setLayer1(SSLayer* layer) { setLayer(1, layer); }
    void setLayer2(SSLayer* layer) { setLayer(2, layer); }
    void setLayer3(SSLayer* layer) { setLayer(3, layer); }
    void setLayer4(SSLayer* layer) { setLayer(4, layer); }
    void setLayer5(SSLayer* layer) { setLayer(5, layer); }
    void setLayer6(SSLayer* layer) { setLayer(6, layer); }
    void setLayer7(SSLayer* layer) { setLayer(7, layer); }

signals:
    void layer0Changed(int idx, SSLayer* layer);
    void layer1Changed(int idx, SSLayer* layer);
    void layer2Changed(int idx, SSLayer* layer);
    void layer3Changed(int idx, SSLayer* layer);
    void layer4Changed(int idx, SSLayer* layer);
    void layer5Changed(int idx, SSLayer* layer);
    void layer6Changed(int idx, SSLayer* layer);
    void layer7Changed(int idx, SSLayer* layer);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData);
    SSLayer* m_layers[8];
    std::vector<SSLayer*> m_visibleLayers;
    bool m_layersChanged[8];
};
