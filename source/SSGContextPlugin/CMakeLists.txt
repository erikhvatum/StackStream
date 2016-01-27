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

project(SSGContextPlugin)

set(SOURCE_FILES
    ../StackStream.h
    ../SSGTexture.cpp
    ../SSGTexture.h
    SSGContext.h
    SSGContext.cpp
    SSGContextPlugin.h
    SSGContextPlugin.cpp
    SSGQuickLayer.h
    SSGQuickLayer.cpp)
set(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${FREEIMAGE_LIBRARY}
    ${FREEIMAGEPLUS_LIBRARY})

add_library(SSGContextPlugin MODULE ${SOURCE_FILES})
qt5_use_modules(SSGContextPlugin Core Gui Qml OpenGL Quick Widgets)
include(/usr/local/debug/qt5/qtdeclarative/src/quick/scenegraph/qsgcontextplugin_p.h)
