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

#include "common.h"
#include "SSQuickFramebufferObject.h"
#include "SSGFramebufferObjectNode.h"
#include <private/qquickitem_p.h>

class SSQuickFramebufferObjectPrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(SSQuickFramebufferObject)
public:
    SSQuickFramebufferObjectPrivate()
        : followsItemSize(true)
        , mirrorVertically(false)
        , node(0)
    {
    }

    bool followsItemSize;
    bool mirrorVertically;
    mutable SSGFramebufferObjectNode *node;
};

SSQuickFramebufferObject::SSQuickFramebufferObject(QQuickItem *parent) :
    QQuickItem(*new SSQuickFramebufferObjectPrivate, parent)
{
    setFlag(ItemHasContents);
}

void SSQuickFramebufferObject::setTextureFollowsItemSize(bool follows)
{
    Q_D(SSQuickFramebufferObject);
    if (d->followsItemSize == follows)
        return;
    d->followsItemSize = follows;
    emit textureFollowsItemSizeChanged(d->followsItemSize);
}

bool SSQuickFramebufferObject::textureFollowsItemSize() const
{
    Q_D(const SSQuickFramebufferObject);
    return d->followsItemSize;
}

void SSQuickFramebufferObject::setMirrorVertically(bool enable)
{
    Q_D(SSQuickFramebufferObject);
    if (d->mirrorVertically == enable)
        return;
    d->mirrorVertically = enable;
    emit mirrorVerticallyChanged(d->mirrorVertically);
    update();
}

bool SSQuickFramebufferObject::mirrorVertically() const
{
    Q_D(const SSQuickFramebufferObject);
    return d->mirrorVertically;
}

void SSQuickFramebufferObject::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    Q_D(SSQuickFramebufferObject);
    if (newGeometry.size() != oldGeometry.size() && d->followsItemSize)
        update();
}

QSGNode *SSQuickFramebufferObject::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    SSGFramebufferObjectNode *n = static_cast<SSGFramebufferObjectNode *>(node);

    // We only abort if we never had a node before. This is so that we
    // don't recreate the renderer object if the thing becomes tiny. In
    // terms of API it would be horrible if the renderer would go away
    // that easily so with this logic, the renderer only goes away when
    // the scenegraph is invalidated or it is removed from the scene.
    if (!n && (width() <= 0 || height() <= 0))
        return 0;

    Q_D(SSQuickFramebufferObject);

    if (!n) {
        if (!d->node)
            d->node = new SSGFramebufferObjectNode;
        n = d->node;
    }

    if (!n->renderer) {
        n->window = window();
        n->renderer = createRenderer();
        n->renderer->data = n;
        n->quickFbo = this;
        connect(window(), SIGNAL(beforeRendering()), n, SLOT(render()));
        connect(window(), SIGNAL(screenChanged(QScreen*)), n, SLOT(handleScreenChange()));
    }

    n->renderer->synchronize(this);

    QSize minFboSize = d->sceneGraphContext()->minimumFBOSize();
    QSize desiredFboSize(qMax<int>(minFboSize.width(), width()),
                         qMax<int>(minFboSize.height(), height()));

    n->devicePixelRatio = window()->effectiveDevicePixelRatio();
    desiredFboSize *= n->devicePixelRatio;

    if (n->fbo && (d->followsItemSize || n->invalidatePending)) {
        if (n->fbo->size() != desiredFboSize) {
            delete n->fbo;
            n->fbo = 0;
            delete n->msDisplayFbo;
            n->msDisplayFbo = 0;
            n->invalidatePending = false;
        }
    }

    if (!n->fbo) {
        n->fbo = n->renderer->createFramebufferObject(desiredFboSize);

        GLuint displayTexture = n->fbo->texture();

        if (n->fbo->format().samples() > 0) {
            n->msDisplayFbo = new QOpenGLFramebufferObject(n->fbo->size());
            displayTexture = n->msDisplayFbo->texture();
        }

        n->setTexture(window()->createTextureFromId(displayTexture,
                                                    n->fbo->size(),
                                                    QQuickWindow::TextureHasAlphaChannel));
    }

    n->setTextureCoordinatesTransform(d->mirrorVertically ? QSGSimpleTextureNode::MirrorVertically : QSGSimpleTextureNode::NoTransform);
    n->setFiltering(d->smooth ? QSGTexture::Linear : QSGTexture::Nearest);
    n->setRect(0, 0, width(), height());

    n->scheduleRender();

    return n;
}

bool SSQuickFramebufferObject::isTextureProvider() const
{
    return true;
}

QSGTextureProvider *SSQuickFramebufferObject::textureProvider() const
{
    // When Item::layer::enabled == true, QQuickItem will be a texture
    // provider. In this case we should prefer to return the layer rather
    // than the fbo texture.
    if (QQuickItem::isTextureProvider())
        return QQuickItem::textureProvider();

    Q_D(const SSQuickFramebufferObject);
    QQuickWindow *w = window();
    if (!w || !w->openglContext() || QThread::currentThread() != w->openglContext()->thread()) {
        qWarning("SSQuickFramebufferObject::textureProvider: can only be queried on the rendering thread of an exposed window");
        return 0;
    }
    if (!d->node)
        d->node = new SSGFramebufferObjectNode;
    return d->node;
}

void SSQuickFramebufferObject::releaseResources()
{
    // When release resources is called on the GUI thread, we only need to
    // forget about the node. Since it is the node we returned from updatePaintNode
    // it will be managed by the scene graph.
    Q_D(SSQuickFramebufferObject);
    d->node = 0;
}

void SSQuickFramebufferObject::invalidateSceneGraph()
{
    Q_D(SSQuickFramebufferObject);
    d->node = 0;
}

SSQuickFramebufferObject::Renderer::Renderer()
    : data(0)
{
}

SSQuickFramebufferObject::Renderer::~Renderer()
{
}

QOpenGLFramebufferObject *SSQuickFramebufferObject::Renderer::framebufferObject() const
{
    return data ? ((SSGFramebufferObjectNode *) data)->fbo : 0;
}

void SSQuickFramebufferObject::Renderer::synchronize(SSQuickFramebufferObject *item)
{
    Q_UNUSED(item);
}

void SSQuickFramebufferObject::Renderer::invalidateFramebufferObject()
{
    if (data)
        ((SSGFramebufferObjectNode *) data)->invalidatePending = true;
}

QOpenGLFramebufferObject *SSQuickFramebufferObject::Renderer::createFramebufferObject(const QSize &size)
{
    return new QOpenGLFramebufferObject(size);
}

void SSQuickFramebufferObject::Renderer::update()
{
    if (data)
        ((SSGFramebufferObjectNode *) data)->scheduleRender();
}


// #include "SSQuickFramebufferObject.moc"
