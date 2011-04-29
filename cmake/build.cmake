if (COMMAND cmake_policy)
    if (POLICY CMP0011)
        cmake_policy(SET CMP0011 NEW)
    endif()
endif()

project(VISIONCOM CXX)

enable_testing()

# VISIONCOM_ASSERT(TEST COMMENT_FAIL [COMMENT_PASS=''] [IS_FATAL=FALSE])
macro(VISIONCOM_ASSERT TEST COMMENT_FAIL)
    if (${TEST})
        # ARG2 holds COMMENT_PASS
        if (${ARGC} GREATER 2)
            message(STATUS ${ARGV2})
        endif()
    else()
        set(IS_FATAL 0)
        if (${ARGC} GREATER 3)
            set(IS_FATAL ${ARGV3})
        endif()

        if (${IS_FATAL})
            message(FATAL_ERROR ${COMMENT_FAIL})
        else()
            message(STATUS ${COMMENT_FAIL})
        endif()
    endif()
endmacro()

include(${VISIONCOM_CMAKE_DIRECTORY}/SetupOs.cmake)
include(${VISIONCOM_CMAKE_DIRECTORY}/SetupDirs.cmake)

include(${VISIONCOM_CMAKE_DIRECTORY}/FindIce.cmake)
VISIONCOM_ASSERT(
    ICE_FOUND
    "Looking for Ice - not found. Please install Ice, ** delete CMakeCache.txt **, then re-run CMake."
    "Looking for Ice - found in ${ICE_HOME}"
    1)

include(${VISIONCOM_CMAKE_DIRECTORY}/TestIce.cmake)
VISIONCOM_ASSERT (
    ICE_WORKS
    "Testing Ice - failed. Please check or reinstall it, ** delete CMakeCache.txt **, then re-run CMake."
    "Testing Ice - ok."
    1)

set(ICE_BIN_DIR ${ICE_HOME}/bin)
set(ICE_LIB_DIR ${ICE_HOME}/lib)
set(ICE_INCLUDE_DIR ${ICE_HOME}/include)

include(${VISIONCOM_CMAKE_DIRECTORY}/FindIceUtil.cmake)
include(${VISIONCOM_CMAKE_DIRECTORY}/UseIce.cmake)

# Boost (headers only)
find_package(Boost REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

# tests
find_package(Boost REQUIRED unit_test_framework)
if (Boost_FOUND)
    link_directories(${Boost_LIBRARY_DIRS})
endif()

include(${VISIONCOM_CMAKE_DIRECTORY}/CompileSettings.cmake)
