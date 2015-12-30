#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T17:30:39
#
#-------------------------------------------------

QT       += core gui quick quick-private qml-private core-private gui-private

CONFIG += c++11

TARGET = ThirtyBitSGContextPlugin
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/scenegraph

SOURCES += ThirtyBitSGContextPlugin.cpp \
    ThirtyBitSGContext.cpp

HEADERS += ThirtyBitSGContextPlugin.h \
    ThirtyBitSGContext.h
DISTFILES += ThirtyBitSGContextPlugin.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
