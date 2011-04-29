
include_directories(${VISIONCOM_BASE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
use_ice()

file(GLOB SOURCES *.cpp)
add_executable("producer" ${SOURCES})
executable_settings("producer")
install(TARGETS "producer" DESTINATION ${VISIONCOM_BIN_DIR})
