// The MIT License (MIT)
//
// Copyright (c) 2016 Erik Hvatum
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Authors: Erik Hvatum <ice.rikh@gmail.com>

#include "../common.h"
#include "../SSImage.h"
#include "../SSLayer.h"
#include "../SSView.h"
#include "../Redis.h"
#include <QApplication>
#include <cstdio>
#include <hiredis/hiredis.h>

static QSurfaceFormat fmt;

class MakeImage
  : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE SSImage* makeImage() { return new SSImage(); }
};

static void onApplicationWindowCreated(QObject* object, const QUrl&)
{
    QQuickWindow* stackStreamMainWindow{qobject_cast<QQuickWindow*>(object)};
    if(stackStreamMainWindow && stackStreamMainWindow->objectName() == "stackStreamMainWindow")
    {
        stackStreamMainWindow->setFormat(fmt);
        stackStreamMainWindow->show();
    }
}

static QPointer<RedisCaptiveInst> redisInst;
static QPointer<QQmlApplicationEngine> engine;
static QMetaObject::Connection redisInstStatusChangedConnection;

static void onRedisInstStatusChanged(RedisInst::Status status, RedisInst::Status /*oldStatus*/)
{
    if(status == RedisInst::Closed)
    {
        qFatal("Failed to establish connection to redis instance.");
        return;
    }
    if(status == RedisInst::Ready)
    {
        QObject::disconnect(redisInstStatusChangedConnection);
        RedisConnection* redisConnection{redisInst->makeConnection()};
        redisReply* r = reinterpret_cast<redisReply*>(redisCommand(redisConnection->c(), "PING"));
        QString rqstr(r->str);
        freeReplyObject(r);
        if(rqstr != "PONG")
        {
            qCritical("Did not receive \"PONG\" in response to redis \"PING\" command.");
        }
        else
        {
            engine = new QQmlApplicationEngine();
            engine->rootContext()->setContextProperty("redisConnection", redisConnection);
            QObject::connect(engine.data(), &QQmlApplicationEngine::objectCreated, onApplicationWindowCreated);
            engine->load(QUrl(QStringLiteral("qrc:/StackStream.qml")));
            MakeImage* ssimageFactory{new MakeImage()};
            engine->rootContext()->setContextProperty("ssimageFactory", ssimageFactory);
            engine->setObjectOwnership(ssimageFactory, QQmlEngine::CppOwnership);
        }
    }
}

int main(int argc, char *argv[])
{
    setenv("QMLSCENE_DEVICE", "SSGContextPlugin", 1);
    QApplication app(argc, argv);
    {
        const char ss[] = "StackStream";
        const int ver[] = {1, 0};
        qmlRegisterType<SSImage>(ss, ver[0], ver[1], "SSImage");
        qmlRegisterType<SSLayer>(ss, ver[0], ver[1], "SSLayer");
        qmlRegisterType<SSView>(ss, ver[0], ver[1], "SSView");
        qmlRegisterUncreatableType<RedisConnection>(ss, ver[0], ver[1], "RedisConnection", "A RedisConnection cannot be instantiated from QML.");
        qmlRegisterUncreatableType<RedisInst>(ss, ver[0], ver[1], "RedisInst", "A RedisInst cannot be instantiated from QML.");
        qRegisterMetaType<SSImage::DType>("DType");
        qRegisterMetaType<SSImage::Components>("Components");
        qRegisterMetaType<std::size_t>("std::size_t");
        qRegisterMetaType<RedisInst::Status>("RedisInstStatus");

        fmt.setRenderableType(QSurfaceFormat::OpenGL);
        fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
        fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        fmt.setSwapInterval(1);
        fmt.setVersion(4, 5);
#ifdef ENABLE_GL_DEBUG_LOGGING
        fmt.setOptions(QSurfaceFormat::DebugContext | QSurfaceFormat::DeprecatedFunctions);
#else
        fmt.setOptions(QSurfaceFormat::DeprecatedFunctions);
#endif
        fmt.setStencilBufferSize(8);
        fmt.setSamples(8);
        // We request 30-bit color; if it's not available, Qt automatically falls back to 24-bit
        fmt.setRedBufferSize(10);
        fmt.setGreenBufferSize(10);
        fmt.setBlueBufferSize(10);
        // NB: Requesting 2-bit alpha and getting it on Linux leads to crashes
    //    fmt.setAlphaBufferSize(2);
        QSurfaceFormat::setDefaultFormat(fmt);
    }
    redisInst = new RedisCaptiveInst();
    redisInstStatusChangedConnection = QObject::connect(redisInst, &RedisInst::statusChanged, onRedisInstStatusChanged);
    QScopedPointer<RedisCaptiveInst> redisInst_{redisInst};
    int ret{-1};
    if(redisInst->status() == RedisInst::Initializing)
    {
        ret = app.exec();
#ifdef ENABLE_GL_DEBUG_LOGGING
        if(g_glDebugLogger)
        {
            delete g_glDebugLogger;
            g_glDebugLogger = nullptr;
        }
#endif
    }
    return ret;
}

#include "main.moc"
