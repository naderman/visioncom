# Test that Ice works (from orca robotics)

# This module assumes that Ice is already found and ICE_HOME is defined
# Defines the following variables:
# ICE_WORKS : 1 if test passed, 0 otherwise.

include(${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)

if (NOT DEFINED ICE_HOME)
    message(STATUS "Testing Ice - CANNOT test because ICE_HOME is not defined.")
    set(ICE_WORKS 0)
endif()

if (NOT VISIONCOM_OS_WIN)
    # On 64-bit systems, Ice libraries are installed in lib64.  Is there a way to tell automatically?
    check_include_file_cxx("Ice/Ice.h" ICE_WORKS "-I${ICE_HOME}/include -L${ICE_HOME}/lib -L${ICE_HOME}/lib64 -lIce -lIceUtil")
    if (ICE_WORKS)
        set(ICE_WORKS 1)
    else()
        set(ICE_WORKS 0)
    endif()
else()
    # Windows: there is no easy way to pass separate compile and link options to the macro,
    # so assume we are told the truth
    set(ICE_WORKS 1)
endif()

# Check Ice version
exec_program(${ICE_HOME}/bin/slice2cpp${VISIONCOM_EXE_EXTENSION} ARGS --version OUTPUT_VARIABLE ICE_VERSION)
message(STATUS "Ice version: '${ICE_VERSION}'")

# notice that this allows beta versions, e.g. 3.3b
if (ICE_VERSION MATCHES "3\\.[3-9]")
    set(ICE_VERSION_OK 1)
else()
    set(ICE_VERSION_OK 0)
endif()

VISIONCOM_ASSERT(
    ICE_VERSION_OK
    "Checking Ice version - ${ICE_VERSION} - failed. VisionCom requires Ice v. 3.3.0 or newer."
    "Checking Ice version - ${ICE_VERSION} - ok"
    1)
