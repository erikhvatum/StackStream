#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T17:30:39
#
#-------------------------------------------------

QT += core gui qml quick widgets quick-private qml-private core-private gui-private

CONFIG += c++11

TARGET = Float32SGContextPlugin
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/scenegraph

SOURCES += Float32SGContextPlugin.cpp \
    Float32SGContext.cpp \
    ../Float32SGTexture.cpp \
    Float32SGLayer.cpp

HEADERS += Float32SGContextPlugin.h \
    Float32SGContext.h \
    ../Float32SGTexture.h \
    Float32SGLayer.h
DISTFILES += Float32SGContextPlugin.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
