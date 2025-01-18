option (SAT_OPENGL "SAT_OPENGL support" OFF)

if (${SAT_OPENGL})

    message (STATUS "SAT_OPENGL support enable")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_opengl.c
                             ${CMAKE_SOURCE_DIR}/src/sat_opengl/sat_opengl_window.c
                             ${CMAKE_SOURCE_DIR}/src/sat_opengl/sat_opengl_program.c
                             ${CMAKE_SOURCE_DIR}/src/sat_opengl/sat_opengl_shader.c
                             ${CMAKE_SOURCE_DIR}/src/sat_opengl/sat_opengl_vao.c
                             ${CMAKE_SOURCE_DIR}/src/sat_opengl/sat_opengl_vbo.c
                             ${CMAKE_SOURCE_DIR}/src/sat_opengl/sat_opengl_ebo.c)

    list (APPEND SAT_INCLUDES ${CMAKE_SOURCE_DIR}/include/sat_opengl)
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_opengl.h>\n")

    list (APPEND SAT_LIBRARIES GLEW glfw GL X11 pthread Xrandr Xi dl m)

    install (FILES include/sat_opengl.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

endif ()