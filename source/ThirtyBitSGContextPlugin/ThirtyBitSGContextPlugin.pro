#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T17:30:39
#
#-------------------------------------------------

#QT       += core gui widgets quick quick-private qml-private core-private gui-private
QT += core gui qml quick widgets quick-private qml-private core-private gui-private

CONFIG += c++11

TARGET = ThirtyBitSGContextPlugin
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/scenegraph

SOURCES += ThirtyBitSGContextPlugin.cpp \
    ThirtyBitSGContext.cpp \
    ../ThirtyBitSGTexture.cpp \
    ThirtyBitSGLayer.cpp

HEADERS += ThirtyBitSGContextPlugin.h \
    ThirtyBitSGContext.h \
    ../ThirtyBitSGTexture.h \
    ThirtyBitSGLayer.h
DISTFILES += ThirtyBitSGContextPlugin.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
