
include_directories(${VISIONCOM_BASE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} "${VISIONCOM_BASE_DIR}/src")
use_ice()

file(GLOB SOURCES *.cpp)
add_executable("consumer" ${SOURCES})
executable_settings("consumer")
target_link_ice("consumer")
target_link_libraries("consumer" "VisionComInterfaces")

install(TARGETS "consumer" DESTINATION ${VISIONCOM_BIN_DIR})
