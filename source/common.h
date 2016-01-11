#pragma once

#include <QApplication>
#include <QSharedDataPointer>
#include <QtCore>
#include <QtGui>
#include <QtPlugin>
#include <QtQml>
#include <QtQuick>
#include <QtWidgets>
#include <atomic>
#include <cstdint>
#include <forward_list>
#include <limits>
#include <FreeImagePlus.h>
#include <memory>
#include <stdexcept>

//#ifndef SSGCONTEXTPLUGIN
// #include <Python.h>
//#endif

#ifdef min
 #undef min
#endif

#ifdef max
 #undef max
#endif

#ifdef read
 #undef read
#endif

#ifdef write
 #undef write
#endif

