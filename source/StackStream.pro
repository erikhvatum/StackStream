TEMPLATE = app

QT += qml quick widgets

CONFIG += c++11

SOURCES += main.cpp \
    testdevcppwidget.cpp \
    Image.cpp \
    Layer.cpp \
    LayerRenderer.cpp

PRECOMPILED_HEADER = common.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    testdevcppwidget.h \
    common.h \
    Image.h \
    Layer.h \
    LayerRenderer.h

DISTFILES +=

LIBS += -lfreeimage -lfreeimageplus

