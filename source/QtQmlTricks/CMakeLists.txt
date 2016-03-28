# The MIT License (MIT)
#
# Copyright (c) 2016 Erik Hvatum
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Authors: Erik Hvatum <ice.rikh@gmail.com>

project(QtQmlTricksPlugin)

set(SOURCE_FILES
#    src/qqmlgadgetlistmodel.cpp
#    src/qqmlgadgetlistmodel.h
    src/qmlplugin.cpp
    src/qmlplugin.h
    src/qqmlhelpers.cpp
    src/qqmlhelpers.h
    src/qqmlmodels.h
    src/qqmlobjectlistmodel.cpp
    src/qqmlobjectlistmodel.h
    src/qqmlsvgiconhelper.cpp
    src/qqmlsvgiconhelper.h
    src/qqmlvariantlistmodel.cpp
    src/qqmlvariantlistmodel.h
    src/qqmlvariantlistmodel_p.h
    src/qquickpolygon.cpp
    src/qquickpolygon.h
    src/qtbitstream.h
    src/qtcobs.h
    src/qtjsonpath.h
    import/QtQmlTricks/qmldir
    include/QQmlGadgetListModel
    include/QQmlHelpers
    include/QQmlObjectListModel
    include/QQmlSvgIconHelper
    include/QQmlVariantListModel
    include/QQuickPolygon
    include/QtBitStream
    include/QtCOBS
    include/QtJsonPath
    include/QtQmlTricks
    )

unset(RESOURCES)
qt5_add_resources(RESOURCES import/QtQmlTricks/components.qrc)
add_library(QtQmlTricksPlugin SHARED ${SOURCE_FILES} ${RESOURCES})
qt5_use_modules(QtQmlTricksPlugin Core Gui Qml OpenGL Quick Svg Widgets)
