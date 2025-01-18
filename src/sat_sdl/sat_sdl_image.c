#include <sat_sdl_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static sat_status_t sat_sdl_image_load_bmp (sat_sdl_image_t *object, const char *file);
static sat_status_t sat_sdl_image_load_png (sat_sdl_image_t *object, const char *file);

sat_status_t sat_sdl_image_load (sat_sdl_image_t *object, const char *file)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl image load error");

    switch (object->type)
    {
    case sat_sdl_image_type_bmp:
        status = sat_sdl_image_load_bmp (object, file);
        break;

    case sat_sdl_image_type_png:
        status = sat_sdl_image_load_png (object, file);
        break;
    
    default:
        break;
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

static sat_status_t sat_sdl_image_load_bmp (sat_sdl_image_t *object, const char *file)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl image load bmp error");

    object->handle = SDL_LoadBMP (file);

    if (object->handle != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_sdl_image_load_png (sat_sdl_image_t *object, const char *file)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl image load png error");

    object->handle = IMG_Load (file);

    if (object->handle != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}