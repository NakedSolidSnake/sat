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
void sat_sdl_window_destroy (sat_sdl_window_t *object);

#endif/* SAT_SDL_WINDOW_H_ */
