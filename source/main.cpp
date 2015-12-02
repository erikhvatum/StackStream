#include "common.h"
#include "Image.h"
#include "Layer.h"
#include "testdevcppwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    const char ss[] = "StackStream";
    const int ver[] = {1, 0};
    qmlRegisterType<Image>(ss, ver[0], ver[1], "SSImage");
    qmlRegisterType<Layer>(ss, ver[0], ver[1], "SSLayer");
    qRegisterMetaType<Image::ComponentType>("ComponentType");
    qRegisterMetaType<std::size_t>("std::size_t");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    //TestDevCppWidget* tdcw{new TestDevCppWidget()};
    //tdcw->show();

    return app.exec();
}
