#include <sat_sdl_window.h>

sat_status_t sat_sdl_window_create (sat_sdl_window_t *object, const char *title, uint16_t width, uint16_t height)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl window create error");

    object->window = SDL_CreateWindow (title, 
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       width,
                                       height,
                                       SDL_WINDOW_SHOWN);

    if (object->window != NULL)
        sat_status_set (&status, true, "");
    
    return status;
}

sat_status_t sat_sdl_window_set_background (sat_sdl_window_t *object, sat_sdl_color_t color)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl window set background error");

    SDL_Surface *surface = SDL_GetWindowSurface (object->window);

    if (surface != NULL)
    {
        SDL_FillRect (surface, NULL, SDL_MapRGB (surface->format, color.red, color.green, color.blue));
        sat_status_set (&status, true, "");
    }

    return status;
}

void sat_sdl_window_refresh (sat_sdl_window_t *object)
{
    SDL_UpdateWindowSurface (object->window);
}

void sat_sdl_window_destroy (sat_sdl_window_t *object)
{
    SDL_UpdateWindowSurface (object->window);
}