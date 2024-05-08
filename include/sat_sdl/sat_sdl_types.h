#ifndef SAT_SDL_TYPES_H_
#define SAT_SDL_TYPES_H_

#include <stdint.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} sat_sdl_color_t;

typedef enum 
{
    sat_sdl_image_type_bmp,
    sat_sdl_image_type_png,
} sat_sdl_image_type_t;

#endif/* SAT_SDL_TYPES_H_ */
