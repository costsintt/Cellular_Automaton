cmake_minimum_required(VERSION 3.0.0)
project(gameOfLife VERSION 0.1.0)

find_package(SDL2 REQUIRED)

add_executable(exeForLinux main.c Board.c gridManipul.c App.c Camera.c)

target_link_libraries(exeForLinux SDL2::SDL2 SDL2::SDL2main)