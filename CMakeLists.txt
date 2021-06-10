cmake_minimum_required(VERSION 3.7.2)

project(renderer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

include_directories(src)
include_directories(SFML)
include_directories(eigen)

find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

set(SRC_LIST ${PATH}
			 src/application.cpp
			 src/camera.cpp
			 src/renderer.cpp
			 src/screen.cpp
			 src/triangle.cpp
			 src/world.cpp)
set(LIBRARIES sfml-graphics sfml-window sfml-system)

get_filename_component(FILENAME ${PATH} NAME_WE)

add_executable(${FILENAME} ${SRC_LIST})
target_link_libraries(${FILENAME} ${LIBRARIES})
