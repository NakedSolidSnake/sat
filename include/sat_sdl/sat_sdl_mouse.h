#ifndef SAT_SDL_MOUSE_H_
#define SAT_SDL_MOUSE_H_

typedef enum 
{
    sat_sdl_mouse_event_type_nothing,
    sat_sdl_mouse_event_type_motion,
    sat_sdl_mouse_event_type_button_down,
    sat_sdl_mouse_event_type_button_up
} sat_sdl_mouse_event_type_t;

sat_sdl_mouse_event_type_t sat_sdl_mouse_event_get_type (int event);

#endif/* SAT_SDL_MOUSE_H_ */
