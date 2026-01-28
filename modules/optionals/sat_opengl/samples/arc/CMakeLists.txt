add_executable (arc "")

target_sources (arc PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/main.c
)

target_link_libraries (arc 
    PRIVATE
    sat
)

set_target_properties (arc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/arc)

add_custom_command(TARGET arc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_LIST_DIR}/shaders
    ${CMAKE_BINARY_DIR}/bin/samples/sat_opengl/arc/shaders
)