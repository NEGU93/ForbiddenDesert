# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1)
# CMAKE_POLICY(SET CMP0072 NEW)
# Project name
project(ForbiddenDesert)

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# Finds Allegro using pkgconfig, so it must be configured correctly
find_package(Allegro5 REQUIRED)

include_directories(${ALLEGRO_INCLUDE_DIR})
include_directories("${PROJECT_SOURCE_DIR}")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(
		${PROJECT_SOURCE_DIR}/include
		${PROJECT_SOURCE_DIR}/src
)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )


file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
		"${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
		"${PROJECT_SOURCE_DIR}/src/*.c"
        )

add_executable(ForbiddenDesert ${all_SRCS})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

