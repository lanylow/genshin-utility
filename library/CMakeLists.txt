include_directories(
    "${CMAKE_SOURCE_DIR}/library/inc/"
    "${CMAKE_SOURCE_DIR}/library/lib/"
)

link_directories(
    "${CMAKE_SOURCE_DIR}/library/lib/"
)

file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/library/*.cpp"
    "${CMAKE_SOURCE_DIR}/library/*.hpp"
)

add_library(library SHARED ${SOURCES})

set_target_properties(library PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/library/bin/release/"
)

target_link_libraries(library "minhook/minhook.x64")

add_custom_command(TARGET library POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/library/bin/release/library.dll" "${CMAKE_SOURCE_DIR}/out/"
)

add_custom_target(copy_library DEPENDS library)