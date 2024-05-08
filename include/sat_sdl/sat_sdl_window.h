#ifndef SAT_SDL_WINDOW_H_
#define SAT_SDL_WINDOW_H_

#include <SDL2/SDL.h>
#include <sat_status.h>
#include <sat_sdl_types.h>

typedef struct 
{
    SDL_Window *window;
} sat_sdl_window_t;

sat_status_t sat_sdl_window_create (sat_sdl_window_t *object, const char *title, uint16_t width, uint16_t height);
sat_status_t sat_sdl_window_set_background (sat_sdl_window_t *object, sat_sdl_color_t color);
void sat_sdl_window_set_image (sat_sdl_window_t *object, SDL_Surface *image);
void sat_sdl_window_refresh (sat_sdl_window_t *object);
void sat_sdl_window_destroy (sat_sdl_window_t *object);

#endif/* SAT_SDL_WINDOW_H_ */
