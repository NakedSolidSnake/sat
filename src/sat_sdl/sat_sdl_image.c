#include <sat_sdl_image.h>
#include <SDL2/SDL.h>

sat_status_t sat_sdl_image_load (sat_sdl_image_t *object, const char *file)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl image load error");

    object->handle = SDL_LoadBMP (file);

    if (object->handle != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_sdl_image_unload (sat_sdl_image_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl image unload error");

    if (object->handle != NULL)
    {
        SDL_FreeSurface (object->handle);
        
        sat_status_set (&status, true, "");
    }

    return status;
}