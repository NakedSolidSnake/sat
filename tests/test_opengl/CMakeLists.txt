if (${SAT_OPENGL})
    add_executable (test_sat_opengl_window ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_window.c)
    target_link_libraries (test_sat_opengl_window PUBLIC sat)
    add_test (NAME test_sat_opengl_window COMMAND test_sat_opengl_window ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_opengl_program ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_program.c)
    target_link_libraries (test_sat_opengl_program PUBLIC sat)
    add_test (NAME test_sat_opengl_program COMMAND test_sat_opengl_program ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_opengl_triangle ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_triangle.c)
    target_link_libraries (test_sat_opengl_triangle PUBLIC sat)
    add_test (NAME test_sat_opengl_triangle COMMAND test_sat_opengl_triangle ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_opengl_triangle_indexes ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_triangle_indexes.c)
    target_link_libraries (test_sat_opengl_triangle_indexes PUBLIC sat)
    add_test (NAME test_sat_opengl_triangle_indexes COMMAND test_sat_opengl_triangle_indexes ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_opengl_triangle_uniform ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_triangle_uniform.c)
    target_link_libraries (test_sat_opengl_triangle_uniform PUBLIC sat)
    add_test (NAME test_sat_opengl_triangle_uniform COMMAND test_sat_opengl_triangle_uniform ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_opengl_triangle_texture ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_triangle_texture.c)
    target_link_libraries (test_sat_opengl_triangle_texture PUBLIC sat)
    add_test (NAME test_sat_opengl_triangle_texture COMMAND test_sat_opengl_triangle_texture ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_opengl_triangle_texture2 ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_triangle_texture2.c)
    target_link_libraries (test_sat_opengl_triangle_texture2 PUBLIC sat)
    add_test (NAME test_sat_opengl_triangle_texture2 COMMAND test_sat_opengl_triangle_texture2 ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_opengl_triangle_transform ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_triangle_transform.c)
    target_link_libraries (test_sat_opengl_triangle_transform PUBLIC sat)
    add_test (NAME test_sat_opengl_triangle_transform COMMAND test_sat_opengl_triangle_transform ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_opengl_triangle_fov ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_triangle_fov.c)
    target_link_libraries (test_sat_opengl_triangle_fov PUBLIC sat)
    add_test (NAME test_sat_opengl_triangle_fov COMMAND test_sat_opengl_triangle_fov ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_opengl_cube_rotation ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_cube_rotation.c)
    target_link_libraries (test_sat_opengl_cube_rotation PUBLIC sat)
    add_test (NAME test_sat_opengl_cube_rotation COMMAND test_sat_opengl_cube_rotation ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_opengl_many_cube_rotation ${CMAKE_CURRENT_LIST_DIR}/test_sat_opengl_many_cube_rotation.c)
    target_link_libraries (test_sat_opengl_many_cube_rotation PUBLIC sat)
    add_test (NAME test_sat_opengl_many_cube_rotation COMMAND test_sat_opengl_many_cube_rotation ${CMAKE_CURRENT_LIST_DIR})

endif ()