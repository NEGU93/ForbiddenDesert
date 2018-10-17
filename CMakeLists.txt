# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.6)

# Project name
project(game)

#Add the executable
# add_executable(game ${game_SRCS})

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $(pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5  --libs --cflags)")
# export CXXFLAGS=$(pkg-config allegro-5 allegro_font-5 --libs --cflags)
# allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

# Set the output folder where your program will be created
# set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# Finds Allegro using pkgconfig, so it must be configured correctly
find_package(Allegro5 REQUIRED)
message(ALLEGRO_INCLUDE_DIR: ${ALLEGRO_INCLUDE_DIR})
# Set include and lib dirs.
include_directories(${ALLEGRO_INCLUDE_DIR})

# set(FD_LIBS ${LIBS} ${OBJC_LIBRARIES} ${ALLEGRO_LIBRARIES})
# /usr/include/allegro5/
# include_directories(/usr/include/allegro5/)
# link_directories( /usr/lib/pkgconfig/ )



# The following folder will be included
# include_directories(${PROJECT_SOURCE_DIR}/src)
# include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB all_SRCS
        "*.h"
        "*.cpp"
		"*.c"
        )

add_executable(game ${all_SRCS})
# TARGET_LINK_LIBRARIES(forbidden liballegro.a )
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_LIBRARIES})
message("allegro libraries = " ${ALLEGRO_LIBRARIES})
# target_link_libraries(forbidden ${FD_LIBS})

# INCLUDE_DIRECTORIES( /usr/include/allegro5 )
# LINK_DIRECTORIES(  /usr/lib/pkgconfig )
# TARGET_LINK_LIBRARIES(game liballegro.a )

#Include Allegro
#include_directories(/home/ubuntu/Documents/GitHub/allegro5/build/include)
#include_directories(/home/ubuntu/Documents/GitHub/allegro5/build/lib/Headers)
#link_directories(/home/ubuntu/Documents/GitHub/allegro5/build/lib)
#connect all the libraries you need
#set(game_LIBS liballegro.so liballegro_dialog.so liballegro_image.so)
#target_link_libraries(game ${game_LIBS})
