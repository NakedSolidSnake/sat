#include <sat_sdl_render.h>

sat_status_t sat_sdl_render_create (sat_sdl_render_t *object, sat_sdl_window_t *window)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl render create error");

    object->render = SDL_CreateRenderer (window->window, -1, SDL_RENDERER_ACCELERATED);

    if (object->render != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

void sat_sdl_render_set_color_background (sat_sdl_render_t *object, sat_sdl_color_t color)
{
    SDL_SetRenderDrawColor (object->render, color.red, color.green, color.blue, 0xFF);

    sat_sdl_render_clear (object);
}

void sat_sdl_render_set_texture (sat_sdl_render_t *object, SDL_Texture *texture, sat_sdl_rectangle_t rectangle)
{
    SDL_Rect __rectangle = 
    {
        .x = rectangle.coordinate.x,
        .y = rectangle.coordinate.y,
        .w = rectangle.dimension.width,
        .h = rectangle.dimension.height,
    };

    SDL_RenderCopyEx (object->render, texture, NULL, &__rectangle, 0, NULL, SDL_FLIP_NONE);
}

void sat_sdl_render_set_viewport (sat_sdl_render_t *object, sat_sdl_rectangle_t rectangle)
{
    SDL_Rect __rectangle = 
    {
        .x = rectangle.coordinate.x,
        .y = rectangle.coordinate.y,
        .w = rectangle.dimension.width,
        .h = rectangle.dimension.height
    };

    SDL_RenderSetViewport (object->render, &__rectangle);
}

void sat_sdl_render_clear (sat_sdl_render_t *object)
{
    SDL_RenderClear (object->render);
}

void sat_sdl_render_draw (sat_sdl_render_t *object)
{
    SDL_RenderPresent (object->render);
}

void sat_sdl_render_destroy (sat_sdl_render_t *object)
{
    SDL_DestroyRenderer (object->render);
}