#ifndef SAT_SDL_RENDER_H_
#define SAT_SDL_RENDER_H_

#include "sat_sdl_window.h"

typedef struct 
{
    void *render;
} sat_sdl_render_t;

sat_status_t sat_sdl_render_create (sat_sdl_render_t *object, sat_sdl_window_t *window);
void sat_sdl_render_set_color_background (sat_sdl_render_t *object, sat_sdl_color_t color);
void sat_sdl_render_set_texture (sat_sdl_render_t *object, SDL_Texture *texture, sat_sdl_rectangle_t rectangle);
void sat_sdl_render_set_texture_xy (sat_sdl_render_t *object, SDL_Texture *texture, sat_sdl_rectangle_t rectangle, sat_sdl_coordinate_t coordinate, sat_sdl_flip_t flip);
void sat_sdl_render_set_viewport (sat_sdl_render_t *object, sat_sdl_rectangle_t rectangle);
void sat_sdl_render_clear (sat_sdl_render_t *object);
void sat_sdl_render_draw (sat_sdl_render_t *object);
void sat_sdl_render_destroy (sat_sdl_render_t *object);

#endif/* SAT_SDL_RENDER_H_ */
