if (${SAT_SDL})
    add_executable (test_sat_sdl_init ${CMAKE_CURRENT_LIST_DIR}/test_sat_sdl_init.c)
    target_link_libraries (test_sat_sdl_init PUBLIC sat)
    add_test (NAME test_sat_sdl_init COMMAND test_sat_sdl_init)

    add_executable (test_sat_sdl_images ${CMAKE_CURRENT_LIST_DIR}/test_sat_sdl_images.c)
    target_link_libraries (test_sat_sdl_images PUBLIC sat)
    add_test (NAME test_sat_sdl_images COMMAND test_sat_sdl_images ${CMAKE_CURRENT_LIST_DIR})

    add_executable (test_sat_sdl_event ${CMAKE_CURRENT_LIST_DIR}/test_sat_sdl_event.c)
    target_link_libraries (test_sat_sdl_event PUBLIC sat)
    add_test (NAME test_sat_sdl_event COMMAND test_sat_sdl_event ${CMAKE_CURRENT_LIST_DIR})

endif ()