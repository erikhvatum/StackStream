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

#pragma once
#include "common.h"
#include "SSQuickFramebufferObject.h"

class SSLayer;

class SSLayerRenderer
  : public SSQuickFramebufferObject::Renderer,
    protected QOpenGLFunctions
{
public:
    static const QSize sm_defaultFboSize;

    SSLayerRenderer();
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize&) override;
    void synchronize(SSQuickFramebufferObject *item) override;

protected:
    // When m_layer is not valid, m_fboSize is set to sm_defaultFboSize and m_tex.isCreated() is false.
    SSLayer m_layer;
    QSize m_fboSize;
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLTexture* m_tex;
    std::size_t m_texSerial;
    int m_vertCoordLoc, m_texLoc, m_viewportSizeLoc;
    int m_rescaleMinLoc, m_rescaleRangeLoc, m_gammaLoc, m_tintLoc;
    static const QVector<QVector2D> sm_quad;
    struct componentCountFormats {
        QOpenGLTexture::TextureFormat texFormat;
        QOpenGLTexture::PixelFormat srcPixelFormat;
    };
    static const componentCountFormats sm_componentCountFormats[];
    static const QOpenGLTexture::PixelType sm_componentPixelTypes[];
};
