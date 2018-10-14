# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# projects name
project(ForbiddenDesert)

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard
# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Finds Allegro using pkgconfig, so it must be configured correctly
find_package(Allegro5 REQUIRED)

# Set include and lib dirs.
include_directories(${ALLEGRO_INCLUDE_DIR})
set(FD_LIBS ${LIBS} ${OBJC_LIBRARIES} ${ALLEGRO_LIBRARIES})


# The following folder will be included
# include_directories(${PROJECT_SOURCE_DIR}/src)
# include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB all_SRCS
        "*.h"
        "*.cpp"
        )

add_executable(forbidden ${all_SRCS})
target_link_libraries(forbidden ${FD_LIBS})
