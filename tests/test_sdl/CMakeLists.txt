if (${SAT_SDL})
    add_executable (test_sat_sdl_init ${CMAKE_CURRENT_LIST_DIR}/test_sat_sdl_init.c)
    target_link_libraries (test_sat_sdl_init PUBLIC sat)
    add_test (NAME test_sat_sdl_init COMMAND test_sat_sdl_init)

    add_executable (test_sat_sdl_bmp ${CMAKE_CURRENT_LIST_DIR}/test_sat_sdl_bmp.c)
    target_link_libraries (test_sat_sdl_bmp PUBLIC sat)
    add_test (NAME test_sat_sdl_bmp COMMAND test_sat_sdl_bmp ${CMAKE_CURRENT_LIST_DIR})
endif ()