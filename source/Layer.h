#pragma once
#include "common.h"
#include "Image.h"

class Layer
  : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)
    Q_PROPERTY(Image* image READ image WRITE setImage NOTIFY imageChanged)
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

    double min() const;
    void setMin(double min);

    double max() const;
    void setMax(double max);

    double gamma() const;
    void setGamma(double gamma);

signals:
    void isValidChanged(bool);
    // imageChanged Indicates that the memory address stored in m_image has changed.  If you are interested in
    // change signals from m_image itself, connect to its signals (EG layer_instance->image()->dataChanged).
    void imageChanged();
    void minChanged(double);
    void maxChanged(double);
    void gammaChanged(double);

public slots:
    void aboutQt() const;

protected:
    Image* m_image;
    double m_min;
    double m_max;
    double m_gamma;
    std::forward_list<QMetaObject::Connection> m_imageSignalConnections;
};
