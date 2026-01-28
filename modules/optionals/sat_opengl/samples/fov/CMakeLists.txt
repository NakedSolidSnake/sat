add_executable (fov "")

target_sources (fov PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/main.c
)

target_link_libraries (fov 
    PRIVATE
    sat
)

set_target_properties (fov PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/fov)

add_custom_command(TARGET fov POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_LIST_DIR}/shaders
    ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/fov/shaders
)

add_custom_command(TARGET fov POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_LIST_DIR}/assets
    ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/fov/assets
)