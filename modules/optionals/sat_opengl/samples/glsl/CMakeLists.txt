add_executable (glsl "")

target_sources (glsl PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/main.c
)

target_link_libraries (glsl 
    PRIVATE
    sat
)

set_target_properties (glsl PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/glsl)

add_custom_command(TARGET glsl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_LIST_DIR}/shaders
    ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/glsl/shaders
)