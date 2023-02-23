cmake_minimum_required(VERSION 3.2.0)
project(grog VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
     set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
#include_directories(${PARENT_DIR}/widgets)

#set(SOURCES main.c Board.c gridManipul.c App.c Camera.c Text.c Board.h gridManipul.h App.h Camera.h Text.h)
add_executable(exeForLinux main.c Board.c gridManipul.c App.c Camera.c Text.c eLists.c
Board.h gridManipul.h App.h Camera.h Text.h eLists.h
kiss_draw.c kiss_posix.c kiss_general.c kiss_widgets.c kiss_sdl.h)

target_link_libraries(exeForLinux SDL2::Main SDL2_ttf SDL2::Image)