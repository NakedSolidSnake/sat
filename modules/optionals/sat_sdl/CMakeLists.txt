if (${SAT_SDL})

    check_library (SDL2 "sdl2")
    check_library (SDL2_IMAGE "SDL2_image")
    check_library (SDL2_TTF "SDL2_ttf")
    check_library (SDL2_MIXER "SDL2_mixer")
    check_library (SDL2_GFX "SDL2_gfx")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()
