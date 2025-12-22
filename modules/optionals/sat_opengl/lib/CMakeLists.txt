add_library (sat_opengl "")

target_sources (sat_opengl
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_opengl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_opengl_window.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_opengl_program.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_opengl_shader.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_opengl_vao.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_opengl_vbo.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_opengl_ebo.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_opengl_texture.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_opengl_container_texture.c
)

target_include_directories (sat_opengl
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/public
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/private
)

target_link_libraries (sat_opengl
    PUBLIC
    sat_set
    sat_file
    GLEW
    glfw
    GL
    X11
    pthread
    Xrandr
    Xi
    dl
    m
)

install (FILES include/sat_opengl.h       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_opengl_types.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_opengl.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_OPTIONAL_LIBS "sat_opengl")