#include <sat_sdl_animate.h>
#include <string.h>

sat_status_t sat_sdl_animate_load (sat_sdl_animate_t *object, sat_sdl_render_t *render, sat_sdl_image_t *image)
{
    return sat_sdl_texture_create (&object->texture, render, image);
}

sat_status_t sat_sdl_animate_add_state (sat_sdl_animate_t *object, char *state, sat_sdl_frame_position_t *positions, uint8_t size)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl animate add error");

    if (object->frames.amount < SAT_SDL_ANIMATE_FRAME_AMOUNT)
    {

        sat_sdl_animate_frame_t *frame = &object->frames.list [object->frames.amount];

        frame->rectangles = calloc (1, sizeof (sat_sdl_rectangle_t) * size);

        if (frame->rectangles != NULL)
        {
            frame->state = state;
            frame->size = size;

            for (uint8_t i = 0; i < size; i++)
            {
                frame->rectangles [i].coordinate.x = positions [i].column * object->dimension.width;
                frame->rectangles [i].coordinate.y = positions [i].row * object->dimension.height;
                frame->rectangles [i].dimension.height = object->dimension.height;
                frame->rectangles [i].dimension.width = object->dimension.width;
            }

            object->frames.amount ++;

            sat_status_set (&status, true, "");
        }
    }

    return status;
}

bool sat_sdl_animate_get_frame (sat_sdl_animate_t *object, char *state, uint8_t index, sat_sdl_rectangle_t **rectangle)
{
    bool status = false;

    for (uint8_t i = 0; i < object->frames.amount; i ++)
    {
        sat_sdl_animate_frame_t *frame = &object->frames.list [i];

        if (index < frame->size && strcmp (frame->state, state) == 0)
        {
            *rectangle = &frame->rectangles [index];

            status = true;

            break;
        }
    }

    return status;
}

void sat_sdl_animate_unload (sat_sdl_animate_t *object)
{
    for (uint8_t i = 0; i < object->frames.amount; i ++)
    {
        free (object->frames.list [i].rectangles);
    }

    return sat_sdl_texture_destroy (&object->texture);
}