cmake_minimum_required(VERSION 3.23)

project(genshin-utility)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /sdl /MP /O2")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /INCREMENTAL:NO")

add_subdirectory(library)