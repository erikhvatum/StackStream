#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtGui>
#include <QtQml>

#include "layer.h"
#include "testmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Layer>("StackStream", 1, 0, "Layer");
    qmlRegisterType<TestModel>("StackStream", 1, 0, "TestModel");
    QQmlApplicationEngine engine;
    TestModel* test_model = new TestModel();
    Layer* layer = new Layer();
    layer->m_gamma = 0.8;
    layer->m_tint = QColor(Qt::red);
    test_model->m_layers.push_back(layer);
    layer = new Layer();
    layer->m_gamma = 0.6;
    layer->m_tint = QColor(Qt::blue);
    test_model->m_layers.push_back(layer);
    engine.rootContext()->setContextProperty("test_model", test_model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

