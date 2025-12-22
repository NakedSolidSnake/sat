#include <sat_sdl_geometry.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

static void sat_sdl_geometry_set_color (sat_sdl_render_t *object, sat_sdl_color_t *color);

void sat_sdl_geometry_draw_point (sat_sdl_render_t *object, sat_sdl_point_t point)
{
    sat_sdl_geometry_set_color (object, &point.color);

    SDL_RenderDrawPoint (object->render, point.coordinate.x, point.coordinate.y);
}

void sat_sdl_geometry_draw_line (sat_sdl_render_t *object, sat_sdl_line_t line)
{
    thickLineRGBA (object->render,
                   line.start.x,
                   line.start.y,
                   line.end.x,
                   line.end.y,
                   line.width,
                   line.color.red,
                   line.color.green,
                   line.color.blue,
                   line.color.alpha);
}

void sat_sdl_geometry_draw_rectangle (sat_sdl_render_t *object, sat_sdl_rectangle_t rectangle)
{
    SDL_Rect __rectangle = 
    {
        .x = rectangle.coordinate.x,
        .y = rectangle.coordinate.y,
        .w = rectangle.dimension.width,
        .h = rectangle.dimension.height
    };

    sat_sdl_geometry_set_color (object, &rectangle.color);

    rectangle.fill == true ? 
        SDL_RenderFillRect (object->render, &__rectangle) :
        SDL_RenderDrawRect (object->render, &__rectangle);
}

void sat_sdl_geometry_draw_circle (sat_sdl_render_t *object, sat_sdl_circle_t circle)
{
    circle.fill == true ?
        filledCircleRGBA (object->render,
                          circle.coordinate.x,
                          circle.coordinate.y,
                          circle.radius,
                          circle.color.red,
                          circle.color.green,
                          circle.color.blue,
                          circle.color.alpha) : 
        circleRGBA (object->render,
                    circle.coordinate.x,
                    circle.coordinate.y,
                    circle.radius,
                    circle.color.red,
                    circle.color.green,
                    circle.color.blue,
                    circle.color.alpha);
}

static void sat_sdl_geometry_set_color (sat_sdl_render_t *object, sat_sdl_color_t *color)
{
    SDL_SetRenderDrawColor (object->render,
                            color->red,
                            color->green,
                            color->blue,
                            color->alpha);
}