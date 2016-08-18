cmake_minimum_required(VERSION 2.8)
project(pincode)

set(COMMON_DIR C:/Users/david/workspace/common)
set(CMAKE_INSTALL_PREFIX ${COMMON_DIR})

if(WIN32)
	set(wxWidgets_ROOT_DIR C:/wxwidgets-3.1.0)
endif()

find_package(wxWidgets REQUIRED base core)

include( ${wxWidgets_USE_FILE} )
include_directories( ./src ${COMMON_DIR}/include)
link_directories( ${COMMON_DIR}/lib )

file(GLOB SOURCES ./src/*.cpp)

add_executable(pincode ${SOURCES})

target_link_libraries(pincode ${wxWidgets_LIBRARIES} nanomsg)

if(WIN32)
	set_target_properties(pincode PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

install(TARGETS pincode RUNTIME DESTINATION bin)