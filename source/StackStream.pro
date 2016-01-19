TEMPLATE = app

QT += qml quick widgets quick-private qml-private core-private gui-private

CONFIG += c++11

SOURCES += main.cpp \
    SSLayer.cpp \
    SSImage.cpp \
    SSLayerRenderer.cpp \
    SSView.cpp \
    SSGTexture.cpp \
    SSGTextureMaterial.cpp \
    GilLocker.cpp \
    SSGFramebufferObjectNode.cpp \
    SSQuickFramebufferObject.cpp

PRECOMPILED_HEADER = common.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    common.h \
    SSLayer.h \
    SSImage.h \
    SSLayerRenderer.h \
    SSView.h \
    SSGTexture.h \
    SSGTextureMaterial.h \
    GilLocker.h \
    SSGFramebufferObjectNode.h \
    SSQuickFramebufferObject.h

DISTFILES +=

LIBS += -lfreeimage -lfreeimageplus #-lpython3.5dm -lpthread -ldl  -lutil -lm

#QMAKE_INCDIR += /usr/local/debug/include/python3.5dm
#QMAKE_LIBDIR += /usr/local/debug/lib
