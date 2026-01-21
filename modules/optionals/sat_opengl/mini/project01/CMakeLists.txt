add_executable (grid "")

target_sources (grid PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/main.c
)

target_link_libraries (grid 
    PRIVATE
    sat
)

set_target_properties (grid PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/mini/grid)

add_custom_command(TARGET grid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/shaders
    ${CMAKE_BINARY_DIR}/bin/mini/grid/shaders
)