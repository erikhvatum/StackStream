cmake_minimum_required(VERSION 3.3)
project(StackStream)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(FreeImage REQUIRED)
find_package(FreeImagePlus REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Core Gui OpenGL Qml Quick Widgets)

add_subdirectory(source)