#pragma once

class QSGContext;
class QSGLayer;

class Float32SGContext
  : public QSGContext
{
public:
    explicit Float32SGContext(QObject* parent=nullptr);

    QSGLayer* createLayer(QSGRenderContext* renderContext) override;
};
