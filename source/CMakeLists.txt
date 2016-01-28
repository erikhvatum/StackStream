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

set(SOURCE_FILES
    GilLocker.cpp
    GilLocker.h
    RedisInst.cpp
    RedisInst.h
    SSGFramebufferObjectNode.cpp
    SSGFramebufferObjectNode.h
    SSGSimpleTextureNode.cpp
    SSGSimpleTextureNode.h
    SSGTexture.cpp
    SSGTexture.h
    SSGTextureMaterial.cpp
    SSGTextureMaterial.h
    SSImage.cpp
    SSImage.h
    SSLayer.cpp
    SSLayer.h
    SSLayerRenderer.cpp
    SSLayerRenderer.frag
    SSLayerRenderer.h
    SSMainWindow.cpp
    SSMainWindow.h
    SSQuickFramebufferObject.cpp
    SSQuickFramebufferObject.h
    SSView.cpp
    SSView.h
    StackStream.h)
set(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${FREEIMAGE_LIBRARY}
    ${FREEIMAGEPLUS_LIBRARY})

qt5_add_resources(RESOURCES StackStream.qrc)
add_library(StackStream SHARED ${SOURCE_FILES} ${RESOURCES})
qt5_use_modules(StackStream Core Gui Qml OpenGL Quick Widgets)
target_link_libraries(StackStream ${LIBRARIES})
set_target_properties(StackStream PROPERTIES COMPILE_OPTIONS "-DSTACKSTREAM")

add_subdirectory(Application)
add_subdirectory(SSGContextPlugin)
if(BUILD_STACKSTREAM_PYTHON_MODULE)
    add_subdirectory(PythonModule)
endif()
