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

#include "SSImageItem.h"
#include "SSGSimpleTextureNode.h"

SSImageItem::SSImageItem(QQuickItem* parent)
  : QQuickItem(parent),
    m_imageSerialSet(false)
{
    setFlag(ItemHasContents);
}

SSImage* SSImageItem::image()
{
    return m_image.data();
}

const SSImage* SSImageItem::image() const
{
    return m_image.data();
}

static void noopDeleter(SSImage*) {}

void SSImageItem::setImage(SSImage* image)
{
    if(image != m_image.data())
    {
        if(image)
        {
            m_image = image->sharedFromThis();
            if(!m_image) m_image = QSharedPointer<SSImage>(image, noopDeleter);
            setImplicitSize(m_image->size().width(), m_image->size().height());
        }
        else
        {
            m_image.reset();
            setImplicitSize(0,0);
        }
        imageChanged();
        update();
    }
}

QSGNode* SSImageItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData)
{
    SSGSimpleTextureNode* n{static_cast<SSGSimpleTextureNode*>(oldNode)};
    if(m_image)
    {
        if(!n) n = new SSGSimpleTextureNode();
        if(!m_imageSerialSet || m_imageSerial != m_image->serial())
        {
            m_imageSerialSet = true;
            m_imageSerial = m_image->serial();
            SSGTexture* t = SSGTexture::fromImage(m_image.data());
            n->setTexture(t);
        }
        n->setRect(0,0,implicitWidth(),implicitHeight());
    }
    else
    {
        if(n)
        {
            delete n;
            n = nullptr;
        }
        m_imageSerialSet = false;
    }
    return n;
}
