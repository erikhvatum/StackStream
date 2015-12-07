#include "common.h"
#include "Image.h"
#include "Layer.h"
#include "View.h"

static QSurfaceFormat fmt;

static void onApplicationWindowCreated(QObject* object, const QUrl&)
{
    QQuickWindow* stackStreamMainWindow{qobject_cast<QQuickWindow*>(object)};
    if(stackStreamMainWindow && stackStreamMainWindow->objectName() == "stackStreamMainWindow")
    {
        stackStreamMainWindow->setFormat(fmt);
//        stackStreamMainWindow->setPersistentOpenGLContext(true);
//        stackStreamMainWindow->setPersistentSceneGraph(true);
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
    qmlRegisterType<View>(ss, ver[0], ver[1], "View");
    qRegisterMetaType<Image::ComponentType>("ComponentType");
    qRegisterMetaType<std::size_t>("std::size_t");

    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    fmt.setSwapInterval(1);
    fmt.setVersion(4, 5);
    fmt.setOptions(QSurfaceFormat::DebugContext | QSurfaceFormat::DeprecatedFunctions);
    fmt.setStencilBufferSize(8);
    fmt.setSamples(8);
    // We request 30-bit color; if it's not available, Qt automatically falls back to 24-bit
    fmt.setRedBufferSize(10);
    fmt.setGreenBufferSize(10);
    fmt.setBlueBufferSize(10);
    // NB: Requesting 2-bit alpha and getting it on Linux leads to crashes
//    fmt.setAlphaBufferSize(2);
    QSurfaceFormat::setDefaultFormat(fmt);
    

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, onApplicationWindowCreated);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
//    QQuickView view(QUrl(QStringLiteral("qrc:/main.qml")));
//    view.setFormat(fmt);
//    view.show();

    return app.exec();
}
