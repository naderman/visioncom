
include_directories(${VISIONCOM_BASE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} "${VISIONCOM_BASE_DIR}/src")
use_ice()

file(GLOB SOURCES *.cpp)
add_executable("producer" ${SOURCES})
executable_settings("producer")
target_link_ice("producer")
target_link_libraries("producer" "VisionComInterfaces")

install(TARGETS "producer" DESTINATION ${VISIONCOM_BIN_DIR})
