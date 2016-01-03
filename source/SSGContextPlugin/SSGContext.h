#pragma once

class QSGContext;
class QSGLayer;

class SSGContext
  : public QSGContext
{
public:
    explicit SSGContext(QObject* parent=nullptr);

    QSGLayer* createLayer(QSGRenderContext* renderContext) override;
};
