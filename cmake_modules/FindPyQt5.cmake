# The MIT License (MIT)
#
# Copyright (c) 2014 WUSTL ZPLAB
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

# Do find_package(PythonInterp) or otherwise set PYTHON_EXECUTABLE before using this module. 
# Do find_package(SIP) or otherwise set SIP_DEFAULT_SIP_DIR before using this module. 
#
# This module may do more in the future if I need it to, such as setting PYQT5_PYUIC5.  However, right now, it just sets
# the following variables:
#
# PYQT5_FOUND - System has PyQt5.
#
# PYQT5_SIP_FLAGS - Value of PyQt5.QtCore.PYQT_CONFIGURATION['sip_flags'], a string containing the -t and -x flags that
#                   that were used by sip during the PyQt5 build process (ie, PyQt5's configure.py script called sip
#                   with these -t and -x parameters while building PyQt5).  C++ PyQt5 extensions are advised to use the
#                   same -t and -x arguments when calling sip.
#
# PYQT5_SIP_DIR - Location of PyQt5 .sip files.  Unlike PyQt4, which offers an API for determining this path, PyQt5
#                 does... not...........  So, we check the default sip includes directory, and if we don't find our
#                 PyQt5 .sip files, we fail and the user must specify PYQT5_SIP_DIR.

set(PYQT5_FOUND false)
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import PyQt5.QtCore;print(PyQt5.QtCore.PYQT_CONFIGURATION['sip_flags'])"
                RESULT_VARIABLE _PYQT5_SIP_FLAGS_RETRIEVED
                ERROR_VARIABLE _PYQT5_SIP_FLAGS_ERROR
                OUTPUT_VARIABLE PYQT5_SIP_FLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT _PYQT5_SIP_FLAGS_RETRIEVED MATCHES 0)
    if(PyQt5_FIND_REQUIRED)
        message(FATAL_ERROR "Failed to import PyQt5 (which must be done in order to determine the proper -x and -t sip arguments for this system):\n${_PYQT5_SIP_FLAGS_ERROR}")
    endif()
else()
    unset(_PYQT5_SIP_FLAGS_RETRIEVED)
    unset(_PYQT5_SIP_FLAGS_ERROR)
    set(_PYQT5_SIP_FLAGS ${PYQT5_SIP_FLAGS})
    separate_arguments(PYQT5_SIP_FLAGS)
    if(NOT PYQT5_SIP_DIR) # Only attempt to guess value if not specified by user
        if(NOT SIP_DEFAULT_SIP_DIR)
            if(PyQt5_FIND_REQUIRED})
                message(FATAL_ERROR "Neither PYQT5_SIP_DIR nor SIP_DEFAULT_SIP_DIR are set.  It is necessary to either, a) call find_package(SIP), or b) set(SIP_DEFAULT_SIP_DIR ...) or call cmake with -DSIP_DEFAULT_DIR=..., or c) set(PYQT5_SIP_DIR ...) or call cmake with -DPYQT5_SIP_DIR=... before attempting to find_package(PyQt5).")
            endif()
        else()
            set(PYQT5_SIP_DIR "${SIP_DEFAULT_SIP_DIR}/PyQt5")
            if(EXISTS "${PYQT5_SIP_DIR}/QtCore/QtCoremod.sip") # Any functional PyQt5 installation includes QtCoreMod.sip
                set(PYQT5_FOUND true)
            else()
                if(APPLE)
                    execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig;print(sysconfig.PREFIX)"
                                    RESULT_VARIABLE _TRY_DIR_RETRIEVED
                                    ERROR_VARIABLE _TRY_DIR_ERROR
                                    OUTPUT_VARIABLE _TRY_DIR
                                    OUTPUT_STRIP_TRAILING_WHITESPACE)
                    if(NOT _TRY_DIR_RETRIEVED MATCHES 0)
                        messge(FATAL_ERROR "Failed to query python prefix:\n${_TRY_DIR_ERROR}")
                    else()
                        set(_TRY_DIR "${_TRY_DIR}/share/sip/PyQt5")
                        message(WARNING "Guessed value for PYQT5_SIP_DIR, \"${PYQT5_SIP_DIR}\", does not seem to be correct.  Trying \"${_TRY_DIR}\".")
                        set(PYQT5_SIP_DIR ${_TRY_DIR})
                        if(EXISTS "${PYQT5_SIP_DIR}/QtCore/QtCoremod.sip")
                            set(PYQT5_FOUND true)
                        endif()
                        unset(_TRY_DIR_RETRIEVED)
                        unset(_TRY_DIR_ERROR)
                        unset(_TRY_DIR)
                    endif()
                endif()
            endif()
            if(NOT PYQT5_FOUND AND PyQt5_FIND_REQUIRED)
                if(APPLE)
                    message(FATAL_ERROR "Could not determine correct value for PYQT5_SIP_DIR.  Please specify a value for PYQT5_SIP_DIR (for example, by calling cmake with -DPYQT5_SIP_DIR=foo/bar).")
                else()
                    message(FATAL_ERROR "Guessed value for PYQT5_SIP_DIR, \"${PYQT5_SIP_DIR}\", does not seem to be correct.  Please specify a value for PYQT5_SIP_DIR (for example, by calling cmake with -DPYQT5_SIP_DIR=foo/bar).")
                endif()
            endif()
        endif()
    else()
        if(NOT EXISTS "${PYQT5_SIP_DIR}/QtCore/QtCoremod.sip")
            if(PyQt5_FIND_REQUIRED)
                message(FATAL_ERROR "Specified or cached value for PYQT5_SIP_DIR, \"${PYQT5_SIP_DIR}\", does not seem to be correct.")
            endif()
        else()
            set(PYQT5_FOUND true)
        endif()
    endif()
endif()

if(PYQT5_FOUND)
    message(STATUS "Found PyQt5:")
    message(STATUS "    PyQt5 sip flags: ${_PYQT5_SIP_FLAGS}")
    message(STATUS "    PyQt5 .sip file directory: ${PYQT5_SIP_DIR}")
endif()

mark_as_advanced(PYQT5_FOUND PYQT5_SIP_FLAGS PYQT5_SIP_DIR)
unset(_PYQT5_SIP_FLAGS)