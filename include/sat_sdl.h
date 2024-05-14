#ifndef SAT_SDL_H_
#define SAT_SDL_H_

#include <sat_status.h>
#include <sat_sdl_types.h>
#include <sat_sdl_events.h>

typedef struct sat_sdl_t sat_sdl_t;

sat_status_t sat_sdl_init (sat_sdl_t **object, const char *title, uint16_t width, uint16_t height);
sat_status_t sat_sdl_set_background (sat_sdl_t *object, sat_sdl_color_t color);
sat_status_t sat_sdl_set_image (sat_sdl_t *object, const char *name);
sat_status_t sat_sdl_image_add (sat_sdl_t *object, char *name, const char *file, sat_sdl_image_type_t type);
sat_status_t sat_sdl_set_event_key_pressed (sat_sdl_t *object, sat_sdl_event_on_key_pressed_t on_key_pressed);
sat_status_t sat_sdl_set_context (sat_sdl_t *object, void *context);
sat_status_t sat_sdl_clear (sat_sdl_t *object);
sat_status_t sat_sdl_draw_point (sat_sdl_t *object, sat_sdl_point_t point);
sat_status_t sat_sdl_draw_line (sat_sdl_t *object, sat_sdl_line_t line);
sat_status_t sat_sdl_draw_rectangle (sat_sdl_t *object, sat_sdl_rectangle_t rectangle);
sat_status_t sat_sdl_draw (sat_sdl_t *object);
sat_status_t sat_sdl_run (sat_sdl_t *object);
sat_status_t sat_sdl_stop (sat_sdl_t *object);
sat_status_t sat_sdl_close (sat_sdl_t *object);

#endif/* SAT_SDL_H_ */
