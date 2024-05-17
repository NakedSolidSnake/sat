#include <sat_sdl_font.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>

sat_status_t sat_sdl_font_load (sat_sdl_font_t *object, uint16_t size, const char *filename)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl font load error");
    
    object->handle = TTF_OpenFont (filename, size);

    if (object->handle != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

void sat_sdl_font_destroy (sat_sdl_font_t *object)
{
    TTF_CloseFont (object->handle);
}