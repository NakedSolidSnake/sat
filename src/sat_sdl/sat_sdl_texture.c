#include <sat_sdl_texture.h>
#include <SDL2/SDL.h>

sat_status_t sat_sdl_texture_create (sat_sdl_texture_t *object, sat_sdl_render_t *render, sat_sdl_image_t *image)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl texture create error");

    object->handle = SDL_CreateTextureFromSurface (render->render, image->handle);

    if (object->handle != NULL)
    {
        object->dimension.height = ((SDL_Surface *)image->handle)->h;
        object->dimension.width  = ((SDL_Surface *)image->handle)->w;
        sat_status_set (&status, true, "");
    }

    return status;
}

void sat_sdl_texture_destroy (sat_sdl_texture_t *object)
{
    SDL_DestroyTexture (object->handle);
}

void sat_sdl_texture_get_dimension (sat_sdl_texture_t *object, sat_sdl_dimension_t *dimension)
{
    dimension->height = object->dimension.height;
    dimension->width = object->dimension.width;
}