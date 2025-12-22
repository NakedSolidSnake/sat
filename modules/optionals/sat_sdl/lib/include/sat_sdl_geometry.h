#ifndef SAT_SDL_GEOMETRY_H_
#define SAT_SDL_GEOMETRY_H_

#include <sat_sdl_render.h>

void sat_sdl_geometry_draw_point (sat_sdl_render_t *object, sat_sdl_point_t point);
void sat_sdl_geometry_draw_line (sat_sdl_render_t *object, sat_sdl_line_t line);
void sat_sdl_geometry_draw_rectangle (sat_sdl_render_t *object, sat_sdl_rectangle_t rectangle);
void sat_sdl_geometry_draw_circle (sat_sdl_render_t *object, sat_sdl_circle_t circle);

#endif/* SAT_SDL_GEOMETRY_H_ */
