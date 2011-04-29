#
# CMakeLists.txt file for VisionCom
#

# project version
set(LIB_NAME       VisionComInterfaces)
set(LIB_VERSION    0.1.0)
set(LIB_SOVERSION  0)

# project vars
set(SLICE2CPP      ${ICE_HOME}/bin/slice2cpp${VISIONCOM_EXE_EXTENSION})
set(SLICE2CPPFLAGS -I${CMAKE_CURRENT_SOURCE_DIR}/slice
                   --stream)
set(DEPEND_FILE ${CMAKE_CURRENT_BINARY_DIR}/slice_depend.txt)

file(GLOB_RECURSE BASE_FILES slice/*.ice)

# setup C++ files in the following loop
set(LIB_FILES "")
set(LIB_HEADER_FILES "")

# run slice on files
foreach(FILE ${BASE_FILES})

    # remove .ice
    string(LENGTH ${FILE} LENGTH)
    math(EXPR LENGTH "${LENGTH} - 4")
    string(SUBSTRING ${FILE} 0 ${LENGTH} FILE)

    file(RELATIVE_PATH FILE "${CMAKE_CURRENT_SOURCE_DIR}/slice/" ${FILE})

    string(REGEX MATCH "^(.+)/[^/]+$" VOID ${FILE})
    #set(DIR ${CMAKE_MATCH_1})
    set(DIR ".")

    set(SLICE_ARGS --dll-export VisionCom_EXPORT --output-dir ${CMAKE_CURRENT_SOURCE_DIR}/cpp/${DIR}
                   --include-dir ${DIR}
                   ${SLICE2CPPFLAGS})

    set(SLICE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/slice/${FILE}.ice)

    execute_process(COMMAND ${SLICE2CPP} ${SLICE_ARGS} ${SLICE_SRC} --depend
                    OUTPUT_FILE ${DEPEND_FILE})
    # parse the file into a list of strings
    file(STRINGS ${DEPEND_FILE} SLICE_DEPENDS_DIRTY)
    # IMPORTANT: this magic line is required, no idea why.
    # without it, 'slice_depends' is not a list, has only one element.
    set(SLICE_DEPENDS_DIRTY ${SLICE_DEPENDS_DIRTY})
    # the first element is the generated file, we remove it.
    list(REMOVE_AT SLICE_DEPENDS_DIRTY 0)

    # the rest are the .ice file dependencies. there's at least one: the actual
    # source .ice file.all dependencies end up having a leading "\ "
    # if we don't remove them, add_custom_command() will interpret them as relative commands
    set(SLICE_DEPENDS "")
    foreach(SLICE_DEPEND ${SLICE_DEPENDS_DIRTY})
        string(LENGTH ${SLICE_DEPEND} SLICE_DEPEND_LENGTH)
        math(EXPR SLICE_DEPEND_LENGTH "${SLICE_DEPEND_LENGTH}-1")
        string(SUBSTRING ${SLICE_DEPEND} 1 ${SLICE_DEPEND_LENGTH} SLICE_DEPEND)
        string(STRIP ${SLICE_DEPEND} SLICE_DEPEND)
        list(APPEND SLICE_DEPENDS ${SLICE_DEPEND})
    endforeach()

    # make sure the directory for the generated files exists
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cpp/${DIR})

    add_custom_command(OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/cpp/${FILE}.cpp
                       COMMAND ${SLICE2CPP}
                       ARGS    ${SLICE_ARGS} ${SLICE_SRC}
                       DEPENDS ${SLICE_DEPENDS}
                       COMMENT "-- Generating cpp file from ${SLICE_SRC}")

    list(APPEND LIB_FILES        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/${FILE}.cpp)
    list(APPEND LIB_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/${FILE}.h)
endforeach()

use_ice()

#setup include paths
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpp)

# build library
add_library(${LIB_NAME} SHARED ${LIB_FILES})
library_settings(${LIB_NAME} ${LIB_VERSION} ${LIB_SOVERSION})
target_link_ice(${LIB_NAME})

install(FILES ${LIB_HEADER_FILES} DESTINATION include/${LIB_NAME})

if (VISIONCOM_OS_WIN)
	install(TARGETS ${LIB_NAME} DESTINATION lib)
else()
	install(TARGETS ${LIB_NAME} LIBRARY DESTINATION lib)
endif()
