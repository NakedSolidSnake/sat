option (SAT_SDL "SAT_SDL support" OFF)

if (${SAT_SDL})

    message (STATUS "SAT_SDL support enable")

    check_library (SDL2 "sdl2")
    check_library (SDL2_IMAGE "SDL2_image")
    check_library (SDL2_TTF "SDL2_ttf")
    check_library (SDL2_MIXER "SDL2_mixer")
    check_library (SDL2_GFX "SDL2_gfx")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_sdl.c
                             ${CMAKE_SOURCE_DIR}/src/sat_sdl/sat_sdl_window.c
                             ${CMAKE_SOURCE_DIR}/src/sat_sdl/sat_sdl_keys.c
                             ${CMAKE_SOURCE_DIR}/src/sat_sdl/sat_sdl_image.c
                             ${CMAKE_SOURCE_DIR}/src/sat_sdl/sat_sdl_render.c
                             ${CMAKE_SOURCE_DIR}/src/sat_sdl/sat_sdl_texture.c
                             ${CMAKE_SOURCE_DIR}/src/sat_sdl/sat_sdl_geometry.c
                             ${CMAKE_SOURCE_DIR}/src/sat_sdl/sat_sdl_mouse.c
                             ${CMAKE_SOURCE_DIR}/src/sat_sdl/sat_sdl_font.c
                             ${CMAKE_SOURCE_DIR}/src/sat_sdl/sat_sdl_sound.c
                             ${CMAKE_SOURCE_DIR}/src/sat_sdl/sat_sdl_animate.c)

    list (APPEND SAT_INCLUDES ${CMAKE_SOURCE_DIR}/include/sat_sdl)
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_sdl.h>\n")

    list (APPEND SAT_LIBRARIES SDL2 SDL2_image SDL2_ttf SDL2_mixer SDL2_gfx)

    install (FILES include/sat_sdl.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_sdl/sat_sdl_events.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_sdl/sat_sdl_keys.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_sdl/sat_sdl_mouse.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_sdl/sat_sdl_types.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

endif ()