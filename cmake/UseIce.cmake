
# This macro should be used before adding a target to setup directories
macro(use_ice)

    include_directories(
        ${ICE_HOME}/include
    )

    if (VISIONCOM_OS_WIN)
       if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
            link_directories(
                ${ICE_HOME}/lib/vc100/x64
            )
       else()
            link_directories(
                ${ICE_HOME}/lib/vc100
            )
       endif()
    else()
        link_directories(
            ${ICE_HOME}/lib
            ${ICE_HOME}/lib/x64
        )
    endif()
endmacro()

# This macro needs to be applied after adding a target to link the target
# against the required libraries.
macro(target_link_ice TARGET)

    if (NOT VISIONCOM_OS_WIN)
        target_link_libraries(${TARGET} Ice IceUtil)
        target_link_libraries(${TARGET} IceStorm IceBox IceGrid Glacier2)
    else()
        # windows... have to link to different libs depending on build type
        target_link_libraries(${TARGET}
            optimized Ice      debug Iced
            optimized IceUtil  debug IceUtild)
        target_link_libraries(${TARGET}
            optimized IceStorm debug IceStormd
            optimized IceBox   debug IceBoxd
            optimized IceGrid  debug IceGridd
            optimized Glacier2 debug Glacier2d)
    endif()

endmacro()
