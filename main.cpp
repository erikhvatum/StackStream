#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtGui>
#include <QtQml>

#include "image.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Image>("StackStream", 1, 0, "Image");
//  qmlRegisterType<Layer>("StackStream", 1, 0, "Layer");
//  qmlRegisterType<TestModel>("StackStream", 1, 0, "TestModel");
    QImage qimage{"/Users/ehvatum/Desktop/overlay_liblinear.png"};
    if(qimage.format() != QImage::Format_RGBA8888)
        qimage = qimage.convertToFormat(QImage::Format_RGBA8888);
    QQmlApplicationEngine engine;
    Image* image{new Image(Image::uint8Image, (std::uint8_t*)qimage.bits(), qimage.size(), 4)};
    engine.rootContext()->setContextProperty("image", image);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
    [&](QObject*, const QUrl&)
    {
        qDebug() << engine.rootContext()->contextProperty("image");
    });
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    qDebug() << engine.rootObjects()[0]->findChild<QObject*>("plop");// << engine.rootContext()->contextProperty("image");
    return app.exec();
}
