#ifndef SAT_SDL_IMAGE_H_
#define SAT_SDL_IMAGE_H_

#include "sat_sdl_types.h"
#include <sat.h>

typedef struct 
{
    void *handle;
    sat_sdl_image_type_t type;
} sat_sdl_image_t;

sat_status_t sat_sdl_image_load (sat_sdl_image_t *object, const char *file);
sat_status_t sat_sdl_image_unload (sat_sdl_image_t *object);

#endif/* SAT_SDL_IMAGE_H_ */
