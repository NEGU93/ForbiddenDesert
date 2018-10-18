# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.6)

# Project name
project(game)

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGLU -lGL -lglut")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# Finds Allegro using pkgconfig, so it must be configured correctly
find_package(Allegro5 REQUIRED)

include_directories(${ALLEGRO_INCLUDE_DIR})
include_directories("${PROJECT_SOURCE_DIR}")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )


file(GLOB all_SRCS
        "*.h"
        "*.cpp"
		"*.c"
        )

add_executable(game ${all_SRCS})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

