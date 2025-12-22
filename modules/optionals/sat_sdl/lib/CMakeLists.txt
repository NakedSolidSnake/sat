add_library (sat_sdl "")

target_sources (sat_sdl
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl_window.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl_keys.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl_image.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl_render.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl_texture.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl_geometry.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl_mouse.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl_font.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl_sound.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sdl_animate.c
)

target_include_directories (sat_sdl
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_sdl
    PUBLIC
    sat_status
    SDL2
    SDL2_image
    SDL2_ttf
    SDL2_mixer
    SDL2_gfx
)

install (FILES include/sat_sdl.h        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_sdl_events.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_sdl_keys.h   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_sdl_mouse.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_sdl_types.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_sdl.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_OPTIONAL_LIBS "sat_sdl")