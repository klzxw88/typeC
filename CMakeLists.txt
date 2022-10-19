PROJECT(typeC)

FIND_PACKAGE(PkgConfig REQUIRED)

PKG_CHECK_MODULES(GLIB REQUIRED glib-2.0)
pkg_check_modules(UDEV REQUIRED libudev)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -g")

set(BIN_NAME typeC)
file(GLOB_RECURSE UTILS_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp)
file(GLOB_RECURSE UDEV_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/udev/*.cpp)
file(GLOB_RECURSE CLASS_TYPEC_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/class/typec/*.cpp)
file(GLOB_RECURSE CLASS_PD_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/class/usb_power_delivery/*.cpp)
file(GLOB_RECURSE BUS_TYPEC_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/bus/typec/*.cpp)
file(GLOB_RECURSE BUS_DP_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/bus/typec/displayport/*.cpp)
file(GLOB_RECURSE HANDLER_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/handler/*.cpp)
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/udev)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/handler)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/class/typec)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/class/usb_power_delivery)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/bus/typec)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/bus/typec/displayport)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${JSONCPP_INCLUDE_DIRS})

link_directories(${GLIB_LIBRARY_DIRS})
link_directories(${JSONCPP_LIBRARY_DIRS})

message(STATUS "${UTILS_SRC_FILES} ${CLASS_TYPEC_SRC_FILES} ${CLASS_PD_SRC_FILES} ${BUS_TYPEC_SRC_FILES} ${BUS_DP_SRC_FILES} ${SRC_FILES}")

add_executable(${BIN_NAME} ${UTILS_SRC_FILES} ${CLASS_TYPEC_SRC_FILES} ${CLASS_PD_SRC_FILES} ${BUS_TYPEC_SRC_FILES} ${BUS_DP_SRC_FILES} ${SRC_FILES} )

target_link_libraries(${BIN_NAME}
	${UDEV_LDFLAGS}
	${GLIB_LIBRARIES}
	jsoncpp
	pthread
	)
