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

#pragma once

class SSLayer
  : public SSQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)
    Q_PROPERTY(SSImage* image READ image WRITE setImage)
    Q_PROPERTY(std::size_t imageSerial READ imageSerial NOTIFY imageSerialChanged)
    Q_PROPERTY(float min READ min WRITE setMin RESET resetMin NOTIFY minChanged)
    Q_PROPERTY(float max READ max WRITE setMax RESET resetMax NOTIFY maxChanged)
    Q_PROPERTY(float gamma READ gamma WRITE setGamma RESET resetGamma NOTIFY gammaChanged)
    Q_PROPERTY(QColor tint READ tint WRITE setTint RESET resetTint NOTIFY tintChanged)
public:
    explicit SSLayer(QQuickItem* parent=nullptr);
    explicit SSLayer(const SSLayer& rhs, QQuickItem* parent=nullptr);
    virtual ~SSLayer();
    SSLayer& operator = (const SSLayer& rhs);
    bool operator == (const SSLayer& rhs) const;
    bool operator != (const SSLayer& rhs) const;

    Renderer* createRenderer() const override;

    bool isValid() const;

    SSImage* image();
    const SSImage* image() const;
    void setImage(SSImage* image);

    std::size_t imageSerial() const;

    float min() const;
    void setMin(float min);
    void resetMin();

    float max() const;
    void setMax(float max);
    void resetMax();

    float gamma() const;
    void setGamma(float gamma);
    void resetGamma();

    const QColor& tint() const;
    void setTint(const QColor& tint);
    void resetTint();

    QSGTextureProvider* textureProvider() const override;

    Q_INVOKABLE bool openURL(const QUrl& url);

signals:
    void isValidChanged(bool);
    void imageSerialChanged(std::size_t);
    void minChanged(float);
    void maxChanged(float);
    void gammaChanged(float);
    void tintChanged(QColor);

public slots:
    void aboutQt() const;

protected:
    SSImage* m_image;
    float m_min;
    float m_max;
    float m_gamma;
    QColor m_tint;
    std::forward_list<QMetaObject::Connection> m_imageSignalConnections;

    void onSerialChanged(std::size_t serial);
    void onImageSizeChanged(QSize size);
};
