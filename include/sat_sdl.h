#ifndef SAT_SDL_H_
#define SAT_SDL_H_

#include <sat_status.h>
#include <sat_sdl_types.h>

typedef struct sat_sdl_t sat_sdl_t;

sat_status_t sat_sdl_init (sat_sdl_t **object, const char *title, uint16_t width, uint16_t height);
sat_status_t sat_sdl_set_background (sat_sdl_t *object, sat_sdl_color_t color);
sat_status_t sat_sdl_set_bmp_image (sat_sdl_t *object, const char *file);
sat_status_t sat_sdl_refresh (sat_sdl_t *object);
sat_status_t sat_sdl_run (sat_sdl_t *object);
sat_status_t sat_sdl_close (sat_sdl_t *object);

#endif/* SAT_SDL_H_ */
