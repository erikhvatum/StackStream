// The MIT License (MIT)
//
// Copyright (c) 2016 Erik Hvatum
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Authors: Erik Hvatum <ice.rikh@gmail.com>

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

