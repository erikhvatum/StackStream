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
    common.h
    GilLocker.cpp
    GilLocker.h
    qmldir
    SSGSimpleTextureNode.cpp
    SSGSimpleTextureNode.h
    SSGTexture.cpp
    SSGTexture.h
    SSGTextureMaterial.cpp
    SSGTextureMaterial.h
    SSImage.cpp
    SSImage.h
    SSImageItem.cpp
    SSImageItem.h
    SSQmlPlugin.cpp
    SSQmlPlugin.h
    SSView.cpp
    SSView.h)
set(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${FREEIMAGE_LIBRARY}
    ${FREEIMAGEPLUS_LIBRARY})

qt5_add_resources(RESOURCES StackStreamPlugin.qrc)
add_library(StackStreamPlugin SHARED ${SOURCE_FILES} ${RESOURCES})
qt5_use_modules(StackStreamPlugin Core Gui Qml OpenGL Quick Widgets)
target_link_libraries(StackStreamPlugin ${LIBRARIES})
set_target_properties(StackStreamPlugin PROPERTIES COMPILE_OPTIONS "-DSTACKSTREAM")
#set_target_properties(StackStreamPlugin PROPERTIES COMPILE_OPTIONS "-DENABLE_GL_DEBUG_LOGGING")

add_subdirectory(SSGContextPlugin)
add_subdirectory(Application)
if(BUILD_STACKSTREAM_PYTHON_MODULE)
    add_subdirectory(PythonModule)
endif()

