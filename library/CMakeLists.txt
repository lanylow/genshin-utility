file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/library/*.c")

add_library(library SHARED ${SOURCES})

target_link_libraries(library ${CMAKE_SOURCE_DIR}/library/lib/minhook/minhook.x64.lib)