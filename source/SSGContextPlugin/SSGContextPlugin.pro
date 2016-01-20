#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T17:30:39
#
#-------------------------------------------------

QT += core gui qml quick widgets quick-private qml-private core-private gui-private

CONFIG += c++11

TARGET = SSGContextPlugin
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/scenegraph

DEFINES += SSGCONTEXTPLUGIN

SOURCES += \
    ../SSGTexture.cpp \
    SSGLayer.cpp \
    SSGContextPlugin.cpp \
    SSGContext.cpp

HEADERS += \
    ../SSGTexture.h \
    SSGLayer.h \
    SSGContextPlugin.h \
    SSGContext.h
DISTFILES += SSGContextPlugin.json

unix {
    target.path = /Qt5/lib
    INSTALLS += target
}

QMAKE_INCDIR += /usr/local/include
