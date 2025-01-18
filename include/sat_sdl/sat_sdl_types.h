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
    uint8_t width;
} sat_sdl_line_t;

typedef struct
{
    sat_sdl_coordinate_t coordinate;
    sat_sdl_dimension_t dimension;
    sat_sdl_color_t color;
    bool fill;
} sat_sdl_rectangle_t;

typedef struct
{
    sat_sdl_coordinate_t coordinate;
    sat_sdl_color_t color;
    uint16_t radius;
    bool fill;
} sat_sdl_circle_t;

typedef enum 
{
    sat_sdl_audio_type_music,
    sat_sdl_audio_type_fx,
} sat_sdl_audio_type_t;

typedef enum 
{
    sat_sdl_audio_control_play,
    sat_sdl_audio_control_pause,
    sat_sdl_audio_control_stop,
    sat_sdl_audio_control_resume
} sat_sdl_audio_control_t;

typedef enum 
{
  sat_sdl_background_orientation_vertical,
  sat_sdl_background_orientation_horizontal  
} sat_sdl_background_orientation_t;

typedef enum 
{
    sat_sdl_animate_type_background,
    sat_sdl_animate_type_sprite,
} sat_sdl_animate_type_t;

typedef struct 
{
    sat_sdl_animate_type_t type;
    sat_sdl_image_type_t image_type;
    sat_sdl_background_orientation_t orientation;

    struct 
    {
        sat_sdl_dimension_t dimension;
    } sprites;

} sat_sdl_animate_properties_t;

typedef struct 
{
    uint8_t row;
    uint8_t column;

} sat_sdl_frame_position_t;

typedef enum 
{
    sat_sdl_flip_none,
    sat_sdl_flip_horizontal,
    sat_sdl_flip_vertical,
} sat_sdl_flip_t;

#endif/* SAT_SDL_TYPES_H_ */
