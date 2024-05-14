#ifndef SAT_SDL_TYPES_H_
#define SAT_SDL_TYPES_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} sat_sdl_color_t;

typedef enum 
{
    sat_sdl_image_type_bmp,
    sat_sdl_image_type_png,
} sat_sdl_image_type_t;

typedef struct 
{
    int x;
    int y;
} sat_sdl_coordinate_t;

typedef struct
{
    int width;
    int height;
} sat_sdl_dimension_t;

typedef struct
{
    sat_sdl_coordinate_t coordinate;
    sat_sdl_color_t color;
} sat_sdl_point_t;

typedef struct
{
    sat_sdl_coordinate_t start;
    sat_sdl_coordinate_t end;
    sat_sdl_color_t color;
} sat_sdl_line_t;

typedef struct
{
    sat_sdl_coordinate_t coordinate;
    sat_sdl_dimension_t dimension;
    sat_sdl_color_t color;
    bool fill;
} sat_sdl_rectangle_t;

#endif/* SAT_SDL_TYPES_H_ */
