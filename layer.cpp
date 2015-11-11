#include "layer.h"

Layer::Layer(QObject *parent) : QObject(parent)
{
    resetTint();
    resetGamma();
}

void Layer::resetTint()
{
    m_tint = QColor(Qt::blue);
    tintChanged();
}

void Layer::resetGamma()
{
    m_gamma = 1.0f;
    gammaChanged();
}
