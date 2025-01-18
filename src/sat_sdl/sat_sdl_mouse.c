#include <sat_sdl_mouse.h>
#include <SDL2/SDL.h>

sat_sdl_mouse_event_type_t sat_sdl_mouse_event_get_type (int event)
{
    sat_sdl_mouse_event_type_t type = sat_sdl_mouse_event_type_nothing;

    switch (event)
    {
    case SDL_MOUSEBUTTONDOWN:
        type = sat_sdl_mouse_event_type_button_down;
        break;

    case SDL_MOUSEBUTTONUP:
        type = sat_sdl_mouse_event_type_button_up;
        break;

    case SDL_MOUSEMOTION:
        type = sat_sdl_mouse_event_type_motion;
        break;
    
    default:
        break;
    }

    return type;
}