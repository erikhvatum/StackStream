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

#include "common.h"
#include "GilLocker.h"
/*
GilLocker::GilLocker()
  : m_PyGILState_STATE(PyGILState_Ensure())
{
}

GilLocker::~GilLocker()
{
    if(PyGILState_Check() == 0)
        qFatal("GilLocker::~GilLocker(): About to release the GIL, but the GIL is not "
               "currently held by this thread.");
    PyGILState_Release(m_PyGILState_STATE);
}

GilUnlocker::GilUnlocker()
  : m_pyThreadState(PyEval_SaveThread())
{
    if(m_pyThreadState == nullptr)
        qFatal("GilUnlocker::GilUnlocker(): PyEval_SaveThread() returned nullptr.")
}

GilUnlocker::~GilUnlocker()
{
    PyEval_RestoreThread(m_pyThreadState);
}
*/
