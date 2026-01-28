add_executable (triangle "")

target_sources (triangle PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/sample.c
)

target_link_libraries (triangle 
    PRIVATE
    sat
)

set_target_properties (triangle PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/triangle)

add_custom_command(TARGET triangle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_LIST_DIR}/shaders
    ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/triangle/shaders
)