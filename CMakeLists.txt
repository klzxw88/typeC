PROJECT(typeC)

set(BIN_NAME typeC)
file(GLOB_RECURSE UTILS_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp)
file(GLOB_RECURSE CLASS_TYPEC_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/class/typec/*.cpp)
file(GLOB_RECURSE CLASS_PD_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/class/usb_power_delivery/*.cpp)
file(GLOB_RECURSE BUS_TYPEC_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/bus/typec/*.cpp)
file(GLOB_RECURSE BUS_DP_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/bus/typec/displayport/*.cpp)
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/class/typec)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/class/usb_power_delivery)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/bus/typec)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/bus/typec/displayport)

message(STATUS "${UTILS_SRC_FILES} ${CLASS_TYPEC_SRC_FILES} ${CLASS_PD_SRC_FILES} ${BUS_TYPEC_SRC_FILES} ${BUS_DP_SRC_FILES} ${SRC_FILES}")

add_executable(${BIN_NAME} ${UTILS_SRC_FILES} ${CLASS_TYPEC_SRC_FILES} ${CLASS_PD_SRC_FILES} ${BUS_TYPEC_SRC_FILES} ${BUS_DP_SRC_FILES} ${SRC_FILES} )

target_link_libraries(${BIN_NAME}
	pthread
	)
