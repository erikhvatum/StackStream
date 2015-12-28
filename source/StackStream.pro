TEMPLATE = app

QT += qml quick widgets quick-private qml-private core-private gui-private

CONFIG += c++11

SOURCES += main.cpp \
    Image.cpp \
    Layer.cpp \
    LayerRenderer.cpp \
    View.cpp \
    ThirtyBitImageItem.cpp \
    ThirtyBitSGTexture.cpp

PRECOMPILED_HEADER = common.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    common.h \
    Image.h \
    Layer.h \
    LayerRenderer.h \
    View.h \
    ThirtyBitImageItem.h \
    ThirtyBitSGTexture.h

DISTFILES +=

LIBS += -lfreeimage -lfreeimageplus

