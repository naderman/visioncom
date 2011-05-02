
include_directories(${VISIONCOM_BASE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} "${VISIONCOM_BASE_DIR}/src")
use_ice()

file(GLOB SOURCES *.cpp)
add_executable("producer-raw" ${SOURCES})
executable_settings("producer-raw")
target_link_ice("producer-raw")
target_link_libraries("producer-raw" "VisionComInterfaces")

install(TARGETS "producer-raw" DESTINATION ${VISIONCOM_BIN_DIR})
