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

    virtual SSLayer* layer0() const;
    virtual SSLayer* layer1() const;
    virtual SSLayer* layer2() const;
    virtual SSLayer* layer3() const;
    virtual SSLayer* layer4() const;
    virtual SSLayer* layer5() const;
    virtual SSLayer* layer6() const;
    virtual SSLayer* layer7() const;

    virtual void setLayer0(SSLayer* layer);
    virtual void setLayer1(SSLayer* layer);
    virtual void setLayer2(SSLayer* layer);
    virtual void setLayer3(SSLayer* layer);
    virtual void setLayer4(SSLayer* layer);
    virtual void setLayer5(SSLayer* layer);
    virtual void setLayer6(SSLayer* layer);
    virtual void setLayer7(SSLayer* layer);

signals:
    void layer0Changed(SSLayer* layer);
    void layer1Changed(SSLayer* layer);
    void layer2Changed(SSLayer* layer);
    void layer3Changed(SSLayer* layer);
    void layer4Changed(SSLayer* layer);
    void layer5Changed(SSLayer* layer);
    void layer6Changed(SSLayer* layer);
    void layer7Changed(SSLayer* layer);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData);
    SSLayer* m_layer0;
    SSLayer* m_layer1;
    SSLayer* m_layer2;
    SSLayer* m_layer3;
    SSLayer* m_layer4;
    SSLayer* m_layer5;
    SSLayer* m_layer6;
    SSLayer* m_layer7;
};
