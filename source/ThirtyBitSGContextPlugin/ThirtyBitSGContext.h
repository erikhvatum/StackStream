#pragma once

class QSGContext;
class QSGLayer;

class ThirtyBitSGContext
  : public QSGContext
{
public:
    explicit ThirtyBitSGContext(QObject* parent=nullptr);

    QSGLayer* createLayer(QSGRenderContext* renderContext) override;
};
