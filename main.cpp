#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtGui>
#include <QtQml>

#include "layer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Layer>("StackStream", 1, 0, "Layer");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

