
include_directories(${VISIONCOM_BASE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} "${VISIONCOM_BASE_DIR}/src")
use_ice()

file(GLOB SOURCES *.cpp)
add_executable("consumer-raw" ${SOURCES})
executable_settings("consumer-raw")
target_link_ice("consumer-raw")
target_link_libraries("consumer-raw" "VisionComInterfaces")

install(TARGETS "consumer-raw" DESTINATION ${VISIONCOM_BIN_DIR})
