
include_directories(${VISIONCOM_BASE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
use_ice()

file(GLOB SOURCES *.cpp)
add_executable("consumer" ${SOURCES})
executable_settings("consumer")
install(TARGETS "consumer" DESTINATION ${VISIONCOM_BIN_DIR})
