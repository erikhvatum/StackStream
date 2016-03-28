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

project(StackStreamApplication)

set(SOURCE_FILES
    ../common.h
    main.cpp)
set(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${FREEIMAGE_LIBRARY}
    ${FREEIMAGEPLUS_LIBRARY})

unset(RESOURCES)
qt5_add_resources(RESOURCES Application.qrc)
add_executable(StackStreamApplication ${SOURCE_FILES} ${RESOURCES})
qt5_use_modules(StackStreamApplication Core Gui Qml OpenGL Quick Widgets)
target_link_libraries(StackStreamApplication StackStreamPlugin)
target_link_libraries(StackStreamApplication QtQmlTricksPlugin)

# TODO: Remove this cheap hack, which is, at the moment, very helpful
# for running directly out of a build directory.
cmake_policy(SET CMP0026 OLD)
get_target_property(_SSG_CONTEXT_PLUGIN_LOCATION SSGContextPlugin LOCATION)
get_target_property(_STACKSTREAM_APPLICATION_LOCATION StackStreamApplication LOCATION)
get_target_property(_STACKSTREAM_PLUGIN_LOCATION StackStreamPlugin LOCATION)
get_target_property(_QTQMLTRICKS_PLUGIN_LOCATION QtQmlTricksPlugin LOCATION)

execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from pathlib import Path; print(str(Path('${_STACKSTREAM_APPLICATION_LOCATION}').parent / 'scenegraph'))"
                OUTPUT_VARIABLE _SSG_CONTEXT_PLUGIN_DEST_DPATH
                OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from pathlib import Path; print(str(Path('${_STACKSTREAM_APPLICATION_LOCATION}').parent / 'StackStream'))"
                OUTPUT_VARIABLE _STACKSTREAM_PLUGIN_DEST_DPATH
                OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from pathlib import Path; print(str(Path('${_STACKSTREAM_APPLICATION_LOCATION}').parent / 'QtQmlTricks'))"
                OUTPUT_VARIABLE _QTQMLTRICKS_PLUGIN_DEST_DPATH
                OUTPUT_STRIP_TRAILING_WHITESPACE)

file(MAKE_DIRECTORY ${_SSG_CONTEXT_PLUGIN_DEST_DPATH})
file(MAKE_DIRECTORY ${_STACKSTREAM_PLUGIN_DEST_DPATH})
file(MAKE_DIRECTORY ${_QTQMLTRICKS_PLUGIN_DEST_DPATH})

if(WIN32)
    string(REPLACE "/" "\\" _SSG_CONTEXT_PLUGIN_LOCATION ${_SSG_CONTEXT_PLUGIN_LOCATION})
    string(REPLACE "/" "\\" _SSG_CONTEXT_PLUGIN_DEST_DPATH ${_SSG_CONTEXT_PLUGIN_DEST_DPATH})
    string(REPLACE "/" "\\" _STACKSTREAM_PLUGIN_LOCATION ${_STACKSTREAM_PLUGIN_LOCATION})
    string(REPLACE "/" "\\" _STACKSTREAM_PLUGIN_DEST_DPATH ${_STACKSTREAM_PLUGIN_DEST_DPATH})
    string(REPLACE "/" "\\" _QTQMLTRICKS_PLUGIN_LOCATION ${_QTQMLTRICKS_PLUGIN_LOCATION})
    string(REPLACE "/" "\\" _QTQMLTRICKS_PLUGIN_DEST_DPATH ${_QTQMLTRICKS_PLUGIN_DEST_DPATH})
    string(REPLACE "/" "\\" CMAKE_SOURCE_DIR _CMAKE_SOURCE_DIR)
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND copy /Y ${_SSG_CONTEXT_PLUGIN_LOCATION} ${_SSG_CONTEXT_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND copy /Y ${_STACKSTREAM_PLUGIN_LOCATION} ${_STACKSTREAM_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND copy /Y ${_CMAKE_SOURCE_DIR}\\source\\qmldir ${_STACKSTREAM_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND copy /Y ${_CMAKE_SOURCE_DIR}\\source\\StackStreamMainWindow.qml ${_STACKSTREAM_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND copy /Y ${_QTQMLTRICKS_PLUGIN_LOCATION} ${_QTQMLTRICKS_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND copy /Y ${_CMAKE_SOURCE_DIR}\\source\\QtQmlTricks\\import\\QtQmlTricks\\qmldir ${_QTQMLTRICKS_PLUGIN_DEST_DPATH})

else()
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND cp -fv ${_SSG_CONTEXT_PLUGIN_LOCATION} ${_SSG_CONTEXT_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND cp -fv ${_STACKSTREAM_PLUGIN_LOCATION} ${_STACKSTREAM_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND cp -fv ${CMAKE_SOURCE_DIR}/source/qmldir ${_STACKSTREAM_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND cp -fv ${CMAKE_SOURCE_DIR}/source/StackStreamMainWindow.qml ${_STACKSTREAM_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND cp -fv ${_QTQMLTRICKS_PLUGIN_LOCATION} ${_QTQMLTRICKS_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND cp -fv ${CMAKE_SOURCE_DIR}/source/QtQmlTricks/import/QtQmlTricks/qmldir ${_QTQMLTRICKS_PLUGIN_DEST_DPATH})
    add_custom_command(
        TARGET StackStreamApplication
        POST_BUILD
        COMMAND cp -fv ${CMAKE_SOURCE_DIR}/source/QtQmlTricks/import/QtQmlTricks/*.qml ${_QTQMLTRICKS_PLUGIN_DEST_DPATH})
endif()










#cmake_policy(SET CMP0026 OLD)
#function(echo_target_property tgt prop)
#  # v for value, d for defined, s for set
#  get_property(v TARGET ${tgt} PROPERTY ${prop})
#  get_property(d TARGET ${tgt} PROPERTY ${prop} DEFINED)
#  get_property(s TARGET ${tgt} PROPERTY ${prop} SET)
#  # only produce output for values that are set
#  if(s)
#    message("tgt='${tgt}' prop='${prop}'")
#    message("  value='${v}'")
#    message("  defined='${d}'")
#    message("  set='${s}'")
#    message("")
#  endif()
#endfunction()
#function(echo_target tgt)
#  if(NOT TARGET ${tgt})
#    message("There is no target named '${tgt}'")
#    return()
#  endif()
#  set(props
#    DEBUG_OUTPUT_NAME
#    DEBUG_POSTFIX
#    RELEASE_OUTPUT_NAME
#    RELEASE_POSTFIX
#    ARCHIVE_OUTPUT_DIRECTORY
#    ARCHIVE_OUTPUT_DIRECTORY_DEBUG
#    ARCHIVE_OUTPUT_DIRECTORY_RELEASE
#    ARCHIVE_OUTPUT_NAME
#    ARCHIVE_OUTPUT_NAME_DEBUG
#    ARCHIVE_OUTPUT_NAME_RELEASE
#    AUTOMOC
#    AUTOMOC_MOC_OPTIONS
#    BUILD_WITH_INSTALL_RPATH
#    BUNDLE
#    BUNDLE_EXTENSION
#    COMPILE_DEFINITIONS
#    COMPILE_DEFINITIONS_DEBUG
#    COMPILE_DEFINITIONS_RELEASE
#    COMPILE_FLAGS
#    DEBUG_POSTFIX
#    RELEASE_POSTFIX
#    DEFINE_SYMBOL
#    ENABLE_EXPORTS
#    EXCLUDE_FROM_ALL
#    EchoString
#    FOLDER
#    FRAMEWORK
#    Fortran_FORMAT
#    Fortran_MODULE_DIRECTORY
#    GENERATOR_FILE_NAME
#    GNUtoMS
#    HAS_CXX
#    IMPLICIT_DEPENDS_INCLUDE_TRANSFORM
#    IMPORTED
#    IMPORTED_CONFIGURATIONS
#    IMPORTED_IMPLIB
#    IMPORTED_IMPLIB_DEBUG
#    IMPORTED_IMPLIB_RELEASE
#    IMPORTED_LINK_DEPENDENT_LIBRARIES
#    IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG
#    IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE
#    IMPORTED_LINK_INTERFACE_LANGUAGES
#    IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG
#    IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE
#    IMPORTED_LINK_INTERFACE_LIBRARIES
#    IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG
#    IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE
#    IMPORTED_LINK_INTERFACE_MULTIPLICITY
#    IMPORTED_LINK_INTERFACE_MULTIPLICITY_DEBUG
#    IMPORTED_LINK_INTERFACE_MULTIPLICITY_RELEASE
#    IMPORTED_LOCATION
#    IMPORTED_LOCATION_DEBUG
#    IMPORTED_LOCATION_RELEASE
#    IMPORTED_NO_SONAME
#    IMPORTED_NO_SONAME_DEBUG
#    IMPORTED_NO_SONAME_RELEASE
#    IMPORTED_SONAME
#    IMPORTED_SONAME_DEBUG
#    IMPORTED_SONAME_RELEASE
#    IMPORT_PREFIX
#    IMPORT_SUFFIX
#    INCLUDE_DIRECTORIES
#    INSTALL_NAME_DIR
#    INSTALL_RPATH
#    INSTALL_RPATH_USE_LINK_PATH
#    INTERPROCEDURAL_OPTIMIZATION
#    INTERPROCEDURAL_OPTIMIZATION_DEBUG
#    INTERPROCEDURAL_OPTIMIZATION_RELEASE
#    LABELS
#    LIBRARY_OUTPUT_DIRECTORY
#    LIBRARY_OUTPUT_DIRECTORY_DEBUG
#    LIBRARY_OUTPUT_DIRECTORY_RELEASE
#    LIBRARY_OUTPUT_NAME
#    LIBRARY_OUTPUT_NAME_DEBUG
#    LIBRARY_OUTPUT_NAME_RELEASE
#    LINKER_LANGUAGE
#    LINK_DEPENDS
#    LINK_FLAGS
#    LINK_FLAGS_DEBUG
#    LINK_FLAGS_RELEASE
#    LINK_INTERFACE_LIBRARIES
#    LINK_INTERFACE_LIBRARIES_DEBUG
#    LINK_INTERFACE_LIBRARIES_RELEASE
#    LINK_INTERFACE_MULTIPLICITY
#    LINK_INTERFACE_MULTIPLICITY_DEBUG
#    LINK_INTERFACE_MULTIPLICITY_RELEASE
#    LINK_SEARCH_END_STATIC
#    LINK_SEARCH_START_STATIC
#    LOCATION
#    LOCATION_DEBUG
#    LOCATION_RELEASE
#    MACOSX_BUNDLE
#    MACOSX_BUNDLE_INFO_PLIST
#    MACOSX_FRAMEWORK_INFO_PLIST
#    MAP_IMPORTED_CONFIG_DEBUG
#    MAP_IMPORTED_CONFIG_RELEASE
#    OSX_ARCHITECTURES
#    OSX_ARCHITECTURES_DEBUG
#    OSX_ARCHITECTURES_RELEASE
#    OUTPUT_NAME
#    OUTPUT_NAME_DEBUG
#    OUTPUT_NAME_RELEASE
#    POST_INSTALL_SCRIPT
#    PREFIX
#    PRE_INSTALL_SCRIPT
#    PRIVATE_HEADER
#    PROJECT_LABEL
#    PUBLIC_HEADER
#    RESOURCE
#    RULE_LAUNCH_COMPILE
#    RULE_LAUNCH_CUSTOM
#    RULE_LAUNCH_LINK
#    RUNTIME_OUTPUT_DIRECTORY
#    RUNTIME_OUTPUT_DIRECTORY_DEBUG
#    RUNTIME_OUTPUT_DIRECTORY_RELEASE
#    RUNTIME_OUTPUT_NAME
#    RUNTIME_OUTPUT_NAME_DEBUG
#    RUNTIME_OUTPUT_NAME_RELEASE
#    SKIP_BUILD_RPATH
#    SOURCES
#    SOVERSION
#    STATIC_LIBRARY_FLAGS
#    STATIC_LIBRARY_FLAGS_DEBUG
#    STATIC_LIBRARY_FLAGS_RELEASE
#    SUFFIX
#    TYPE
#    VERSION
#    VS_DOTNET_REFERENCES
#    VS_GLOBAL_WHATEVER
#    VS_GLOBAL_KEYWORD
#    VS_GLOBAL_PROJECT_TYPES
#    VS_KEYWORD
#    VS_SCC_AUXPATH
#    VS_SCC_LOCALPATH
#    VS_SCC_PROJECTNAME
#    VS_SCC_PROVIDER
#    VS_WINRT_EXTENSIONS
#    VS_WINRT_REFERENCES
#    WIN32_EXECUTABLE
#    XCODE_ATTRIBUTE_WHATEVER)
#  message("======================== ${tgt} ========================")
#  foreach(p ${props})
#    echo_target_property("${t}" "${p}")
#  endforeach()
#  message("")
#endfunction()
#function(echo_targets)
#  set(tgts ${ARGV})
#  foreach(t ${tgts})
#    echo_target("${t}")
#  endforeach()
#endfunction()

#echo_targets(SSGContextPlugin)

