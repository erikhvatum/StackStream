#pragma once
#include "common.h"
#include "Image.h"

class Layer
  : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    explicit Layer(QQuickItem* parent=nullptr);
    Renderer* createRenderer() const override;

signals:

public slots:
};
