#pragma once
#include "common.h"
#include "Image.h"

class Layer
  : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)
    Q_PROPERTY(Image* image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(std::size_t imageSerial READ imageSerial NOTIFY imageSerialChanged)
    Q_PROPERTY(double min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(double max READ max WRITE setMax NOTIFY maxChanged)
    Q_PROPERTY(double gamma READ gamma WRITE setGamma NOTIFY gammaChanged)
public:
    explicit Layer(QQuickItem* parent=nullptr);
    explicit Layer(const Layer& rhs, QQuickItem* parent=nullptr);
    Layer& operator = (const Layer& rhs);
    bool operator == (const Layer& rhs) const;

    Renderer* createRenderer() const override;

    bool isValid() const;

    Image* image();
    const Image* image() const;
    void setImage(Image* image);

    const std::size_t& imageSerial() const;

    double min() const;
    void setMin(double min);

    double max() const;
    void setMax(double max);

    double gamma() const;
    void setGamma(double gamma);

signals:
    void isValidChanged(bool);
    // imageChanged indicates that m_image now points to a different image.  IE, m_image is a pointer, and that pointer 
    // value has changed.  If you are interested in change signals from the Image m_image points to, connect directly to
    // its signals (EG layer_instance->image()->sizeChanged), or, for the special case of detecting modification or 
    // replacement of the memory region containing the pixel data of the Image pointed to by m_image, connect to 
    // imageSerialChanged.
    void imageChanged();
    void imageSerialChanged(std::size_t);
    void minChanged(double);
    void maxChanged(double);
    void gammaChanged(double);

public slots:
    void aboutQt() const;

protected:
    Image* m_image;
    std::size_t m_imageSerial;
    double m_min;
    double m_max;
    double m_gamma;
    std::forward_list<QMetaObject::Connection> m_imageSignalConnections;

    void onImageDataChanged();
};
