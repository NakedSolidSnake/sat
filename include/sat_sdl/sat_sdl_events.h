#ifndef SAT_SDL_EVENTS_H_
#define SAT_SDL_EVENTS_H_

#include <sat_sdl_keys.h>
#include <sat_sdl_mouse.h>
#include <sat_sdl_types.h>

typedef void (*sat_sdl_event_on_key_pressed_t) (void *object, sat_sdl_key_t key);
typedef void (*sat_sdl_event_on_key_released_t) (void *object, sat_sdl_key_t key);
typedef void (*sat_sdl_event_on_mouse_event_t) (void *object, sat_sdl_mouse_event_type_t event, sat_sdl_coordinate_t coordinate);


#endif/* SAT_SDL_EVENTS_H_ */
