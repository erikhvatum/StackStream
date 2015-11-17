#pragma once

#include <QtGui>
#include <memory>
#include "image.h"

class Layer
  : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor tint MEMBER m_tint NOTIFY tintChanged RESET resetTint STORED true)
    Q_PROPERTY(qreal gamma MEMBER m_gamma NOTIFY gammaChanged RESET resetGamma STORED true)
public:
    explicit Layer(QObject *parent = 0);
    void resetTint();
    void resetGamma();

signals:
    void tintChanged();
    void gammaChanged();

public:
    std::shared_ptr<Image> m_image;
    QColor m_tint;
    qreal m_gamma;

public slots:
};
