#ifndef SAT_SDL_TEXTURE_H_
#define SAT_SDL_TEXTURE_H_

#include <sat_status.h>
#include <sat_sdl_render.h>
#include <sat_sdl_image.h>

typedef struct
{
    char *name;
    void *handle;

    struct 
    {
        uint16_t width;
        uint16_t height;
    } dimension;
    
} sat_sdl_texture_t;

sat_status_t sat_sdl_texture_create (sat_sdl_texture_t *object, sat_sdl_render_t *render, sat_sdl_image_t *image);
void sat_sdl_texture_destroy (sat_sdl_texture_t *object);
void sat_sdl_texture_get_dimension (sat_sdl_texture_t *object, sat_sdl_dimension_t *dimension);

#endif/* SAT_SDL_TEXTURE_H_ */
