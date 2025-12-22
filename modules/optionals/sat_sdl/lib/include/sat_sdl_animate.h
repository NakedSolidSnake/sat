#ifndef SAT_SDL_ANIMATE_H_
#define SAT_SDL_ANIMATE_H_

#include <sat_sdl_texture.h>
#include <sat_sdl_types.h>

#define SAT_SDL_ANIMATE_FRAME_AMOUNT    20

typedef struct 
{
    const char *state;
    uint8_t size;
    sat_sdl_rectangle_t *rectangles;
    uint8_t current;

} sat_sdl_animate_frame_t;

typedef struct 
{
    sat_sdl_texture_t texture;
    sat_sdl_animate_type_t type;
    sat_sdl_background_orientation_t orientation;
    int32_t offset;

    sat_sdl_dimension_t dimension;
    struct 
    {
        sat_sdl_animate_frame_t list [SAT_SDL_ANIMATE_FRAME_AMOUNT];
        uint8_t amount;
    } frames;

} sat_sdl_animate_t;

sat_status_t sat_sdl_animate_load (sat_sdl_animate_t *object, sat_sdl_render_t *render, sat_sdl_image_t *image);
sat_status_t sat_sdl_animate_add_state (sat_sdl_animate_t *object, char *state, sat_sdl_frame_position_t *positions, uint8_t size);
bool sat_sdl_animate_get_frame (sat_sdl_animate_t *object, char *state, uint8_t index, sat_sdl_rectangle_t **rectangle);
void sat_sdl_animate_unload (sat_sdl_animate_t *object);

#endif/* SAT_SDL_ANIMATE_H_ */
