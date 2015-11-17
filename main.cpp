#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtGui>
#include <QtQml>

#include "image.h"
#include "layer.h"
#include "testmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Image>("StackStream", 1, 0, "Image");
//  qmlRegisterType<Layer>("StackStream", 1, 0, "Layer");
//  qmlRegisterType<TestModel>("StackStream", 1, 0, "TestModel");
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("image", new Image());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

