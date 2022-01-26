file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/loader/src/*.c")

add_executable(loader ${SOURCES})