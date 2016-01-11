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
