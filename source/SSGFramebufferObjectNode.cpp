/****************************************************************************
**
** Portions (C) 2015 The Qt Company Ltd.
** Portions (C) 2016 Erik Hvatum
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQuick module of the Qt Toolkit.
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

#include "StackStream.h"

SSGFramebufferObjectNode::SSGFramebufferObjectNode()
  : window(0),
    fbo(0),
    msDisplayFbo(0),
    renderer(0),
    renderPending(true),
    invalidatePending(false),
    devicePixelRatio(1)
{
    qsgnode_set_description(this, QStringLiteral("fbonode"));
}

SSGFramebufferObjectNode::~SSGFramebufferObjectNode()
{
    delete renderer;
    delete texture();
    delete fbo;
    delete msDisplayFbo;
}

void SSGFramebufferObjectNode::scheduleRender()
{
    renderPending = true;
    window->update();
}

QSGTexture *SSGFramebufferObjectNode::texture() const
{
    return QSGSimpleTextureNode::texture();
}

void SSGFramebufferObjectNode::render()
{
    if (renderPending) {
        renderPending = false;
        fbo->bind();
        QOpenGLContext::currentContext()->functions()->glViewport(0, 0, fbo->width(), fbo->height());
        renderer->render();
        fbo->bindDefault();

        if (msDisplayFbo)
            QOpenGLFramebufferObject::blitFramebuffer(msDisplayFbo, fbo);

        markDirty(QSGNode::DirtyMaterial);
        emit textureChanged();
    }
}

void SSGFramebufferObjectNode::handleScreenChange()
{
    if (window->effectiveDevicePixelRatio() != devicePixelRatio) {
        renderer->invalidateFramebufferObject();
        quickFbo->update();
    }
}
