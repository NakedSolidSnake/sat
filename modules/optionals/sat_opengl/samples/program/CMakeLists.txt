add_executable (program "")

target_sources (program PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/main.c
)

target_link_libraries (program 
    PRIVATE
    sat
)

set_target_properties (program PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/program)

add_custom_command(TARGET program POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_LIST_DIR}/shaders
    ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/program/shaders
)
