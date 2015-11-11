#include "layer.h"

Layer::Layer(QObject *parent) : QObject(parent)
{

}

void Layer::resetTint()
{
    m_tint = QColor(Qt::white);
}

void Layer::resetGamma()
{
    m_gamma = 1.0f;
}
