#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T17:30:39
#
#-------------------------------------------------

QT       += core gui

TARGET = ThirtyBitSGContextPlugin
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/generic

SOURCES += ThirtyBitSGContextPlugin.cpp

HEADERS += ThirtyBitSGContextPlugin.h
DISTFILES += ThirtyBitSGContextPlugin.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
