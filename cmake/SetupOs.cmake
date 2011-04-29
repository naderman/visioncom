#
# OS identification based on gearbox.
#

# CMake does not distinguish Linux from other Unices.
string(REGEX MATCH Linux VISIONCOM_OS_LINUX ${CMAKE_SYSTEM_NAME})

# Rename CMake's variable to something which makes more sense.
if (QNXNTO)
    set(VISIONCOM_OS_QNX TRUE BOOL INTERNAL)
endif()

# In windows we just mirror CMake's own variable
if (WIN32)
    set(VISIONCOM_OS_WIN TRUE BOOL INTERNAL)
endif()

# In MacOS X we just mirror CMake's own variable
if (APPLE)
    set(VISIONCOM_OS_MAC TRUE BOOL INTERNAL)
endif()


# From now on, use our own OS flags

if (VISIONCOM_OS_LINUX)
    message(STATUS "Running on Linux")

    # 32 or 64 bit Linux
    # Set the library directory suffix accordingly
    if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(VISIONCOM_PROC_64BIT TRUE BOOL INTERNAL)
        message(STATUS "Linux x86_64 target detected")
    elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ppc64")
        message(STATUS "Linux ppc64 target detected")
        set(VISIONCOM_PROC_64BIT TRUE BOOL INTERNAL)
    endif()
endif()

if (VISIONCOM_OS_QNX)
    message(STATUS "Running on QNX")
    add_definitions(-shared -fexceptions)
endif()

if (VISIONCOM_OS_WIN)
    # CMake seems not to set this property correctly for some reason
    set(VISIONCOM_EXE_EXTENSION ".exe")
    message(STATUS "Running on Windows")
else()
    set(VISIONCOM_EXE_EXTENSION "")
endif()

if (VISIONCOM_OS_MAC)
    message(STATUS "Running on Mac OS X")
endif()
