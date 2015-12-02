#include "common.h"
#include "Image.h"
#include "Layer.h"

static QSurfaceFormat fmt;

static void onApplicationWindowCreated(QObject* object, const QUrl& url)
{
    QQuickWindow* stackStreamMainWindow{qobject_cast<QQuickWindow*>(object)};
    if(stackStreamMainWindow && stackStreamMainWindow->objectName() == "stackStreamMainWindow")
    {
        stackStreamMainWindow->setFormat(fmt);
        stackStreamMainWindow->show();
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    const char ss[] = "StackStream";
    const int ver[] = {1, 0};
    qmlRegisterType<Image>(ss, ver[0], ver[1], "SSImage");
    qmlRegisterType<Layer>(ss, ver[0], ver[1], "SSLayer");
    qRegisterMetaType<Image::ComponentType>("ComponentType");
    qRegisterMetaType<std::size_t>("std::size_t");

    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    fmt.setSwapInterval(1);
    fmt.setVersion(4, 5);
    fmt.setOptions(/*QSurfaceFormat::DebugContext |*/ QSurfaceFormat::DeprecatedFunctions);
    fmt.setSamples(8);
    fmt.setRedBufferSize(10);
    fmt.setGreenBufferSize(10);
    fmt.setBlueBufferSize(10);
    fmt.setAlphaBufferSize(2);
    QSurfaceFormat::setDefaultFormat(fmt);
    

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, onApplicationWindowCreated);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
//    QQuickView view(QUrl(QStringLiteral("qrc:/main.qml")));
//    view.setFormat(fmt);
//    view.show();

    return app.exec();
}
