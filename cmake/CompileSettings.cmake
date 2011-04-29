
macro(executable_settings EXECUTABLE_NAME)
    message(STATUS "        Configuring executable ${EXECUTABLE_NAME}.")
    compilation_settings()

    message(STATUS "        Using compiler flags (CXX_FLAGS): " ${CXX_FLAGS})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        COMPILE_FLAGS "${CXX_FLAGS}"
        RUNTIME_OUTPUT_DIRECTORY "${VISIONCOM_BIN_DIR}")

    if (VISIONCOM_OS_LINUX)
        get_target_property(EXECUTABLE_FILE ${EXECUTABLE_NAME} LOCATION)
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                        COMMAND strip ARGS --strip-unneeded ${EXECUTABLE_FILE})
    endif()
endmacro()

macro(library_settings LIB_NAME LIB_VERSION LIB_SOVERSION)
    message(STATUS "        Configuring shared library ${LIB_NAME} version ${LIB_VERSION}.")
    compilation_settings()

    if(NOT VISIONCOM_OS_WIN)
        set(CXX_FLAGS "${CXX_FLAGS} -fPIC -D_REENTRANT")
    else()
        add_definitions(-D_USE_MATH_DEFINES)
    endif()

    if (VISIONCOM_OS_WIN)
        add_definitions(-DVisionCom_EXPORTS)
    endif()

    set_target_properties(${LIB_NAME} PROPERTIES
        VERSION   ${LIB_VERSION}
        SOVERSION ${LIB_SOVERSION})

    message(STATUS "        Using compiler flags (CXX_FLAGS): " ${CXX_FLAGS})
    set_target_properties(${LIB_NAME} PROPERTIES
        COMPILE_FLAGS "${CXX_FLAGS}"
        LIBRARY_OUTPUT_DIRECTORY "${VISIONCOM_LIB_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${VISIONCOM_ARCHIVE_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${VISIONCOM_BIN_DIR}"         # DLL path
        )

    if (VISIONCOM_OS_LINUX)
        get_target_property(LIB_FILE ${LIB_NAME} LOCATION)
        add_custom_command(TARGET ${LIB_NAME} POST_BUILD
                        COMMAND strip ARGS --strip-unneeded ${LIB_FILE})
    endif()

endmacro()


macro(compilation_settings)
    # Ice uses long long in C++ which -pedantic turns into an error instead of
    # a warning - we turn it off entirely
    # would really like to use -pedantic here, but Ice breaks it
    set(GENERAL_FLAGS "-Wall -Wextra -Wno-long-long -Wno-unused-parameter")

    if (NOT VISIONCOM_OS_WIN)
        if (CMAKE_BUILD_TYPE STREQUAL Debug)
            set(CXX_FLAGS "-g ${GENERAL_FLAGS}")
        else()
            set(CXX_FLAGS "-O2 ${GENERAL_FLAGS}")
        endif()
    endif()

    # enable multipthreaded compilation on visual studio
    if (VISIONCOM_OS_WIN AND MSVC)
        add_definitions(/MP)
    endif()

endmacro()
