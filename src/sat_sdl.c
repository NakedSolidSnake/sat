#include <sat_sdl.h>
#include <string.h>
#include <stdlib.h>
#include <sat_sdl_window.h>

struct sat_sdl_t
{
    sat_sdl_window_t window;
    uint16_t width;
    uint16_t height;
    bool running;
    bool initialized;

    struct 
    {
        sat_sdl_event_on_key_pressed_t on_key_pressed;
    } events;

    void *context;
}; 

sat_status_t sat_sdl_init (sat_sdl_t **object, const char *title, uint16_t width, uint16_t height)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl init error");

    if (object != NULL && title != NULL && width > 0 && height > 0)
    {
        do 
        {

            if (SDL_Init (SDL_INIT_EVERYTHING) < 0)
            {
                break;
            }

            *object = calloc (1, sizeof (struct sat_sdl_t));
            if (*object == NULL)
            {
                break;
            }

            status = sat_sdl_window_create (&(*object)->window, title, width, height);

            (*object)->running = true;
            (*object)->height = height;
            (*object)->width = width;
            (*object)->initialized = true;

        } while (false);
    }

    return status;
}

sat_status_t sat_sdl_set_background (sat_sdl_t *object, sat_sdl_color_t color)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl set background error");

    if (object != NULL && object->initialized == true)
    {
        sat_sdl_window_set_background (&object->window, color);

        sat_status_set (&status, true, "");
    }

    return status; 
}

sat_status_t sat_sdl_set_bmp_image (sat_sdl_t *object, const char *file)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl set bmp image error");

    if (object != NULL && object->initialized == true && file != NULL)
    {
        sat_sdl_window_set_bmp_image (&object->window, file);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_sdl_set_event_key_pressed (sat_sdl_t *object, sat_sdl_event_on_key_pressed_t on_key_pressed)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl set event key error");

    if (object != NULL && object->initialized == true && on_key_pressed != NULL)
    {
        object->events.on_key_pressed = on_key_pressed;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_sdl_set_context (sat_sdl_t *object, void *context)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl set context error");

    if (object != NULL && object->initialized == true && context != NULL)
    {
        object->context = context;

        sat_status_set (&status, true, "");
    }

    return status;
}


sat_status_t sat_sdl_refresh (sat_sdl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl refresh error");

    if (object != NULL && object->initialized == true)
    {
        sat_sdl_window_refresh (&object->window);

        sat_status_set (&status, true, "");
    }

    return status;    
}

sat_status_t sat_sdl_run (sat_sdl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl run error");
    SDL_Event event;

    if (object != NULL && object->initialized == true)
    {
        while (object->running == true)
        {
            while (SDL_PollEvent (&event))
            {
                if (event.type == SDL_QUIT)
                    object->running = false;

                else if (event.type == SDL_KEYDOWN)
                {
                    if (object->events.on_key_pressed != NULL)
                    {
                        object->events.on_key_pressed (object->context,
                                                       sat_sdl_key_get_by (event.key.keysym.sym));
                    }
                }
            }
        }

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_sdl_stop (sat_sdl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl stop error");

    if (object != NULL && object->initialized == true)
    {
        object->running = false;

        sat_status_set (&status, true, "");
    }

    return status; 
}

sat_status_t sat_sdl_close (sat_sdl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl close error");

    if (object != NULL && object->initialized == true)
    {
        sat_sdl_window_destroy (&object->window);

        free (object);

        SDL_Quit ();

        sat_status_set (&status, true, "");
    }

    return status;
}