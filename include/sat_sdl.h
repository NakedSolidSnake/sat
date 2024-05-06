#ifndef SAT_SDL_H_
#define SAT_SDL_H_

#include <sat_status.h>
#include <sat_sdl_types.h>
#include <sat_sdl_events.h>

typedef struct sat_sdl_t sat_sdl_t;

sat_status_t sat_sdl_init (sat_sdl_t **object, const char *title, uint16_t width, uint16_t height);
sat_status_t sat_sdl_set_background (sat_sdl_t *object, sat_sdl_color_t color);
sat_status_t sat_sdl_set_bmp_image (sat_sdl_t *object, const char *file);
sat_status_t sat_sdl_set_event_key_pressed (sat_sdl_t *object, sat_sdl_event_on_key_pressed_t on_key_pressed);
sat_status_t sat_sdl_set_context (sat_sdl_t *object, void *context);
sat_status_t sat_sdl_refresh (sat_sdl_t *object);
sat_status_t sat_sdl_run (sat_sdl_t *object);
sat_status_t sat_sdl_stop (sat_sdl_t *object);
sat_status_t sat_sdl_close (sat_sdl_t *object);

#endif/* SAT_SDL_H_ */
