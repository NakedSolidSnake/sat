#include <sat_sdl.h>
#include <string.h>
#include <stdlib.h>
#include <sat_sdl_window.h>
#include <sat_sdl_image.h>
#include <SDL2/SDL_image.h>

#define SAT_SDL_IMAGES_SIZE     20

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

    struct 
    {
        sat_sdl_image_t list [SAT_SDL_IMAGES_SIZE];
        uint8_t amount;
    } images;

    void *context;
};

static sat_status_t sat_sdl_init_api (void);
static void sat_sdl_close_api (void);

sat_status_t sat_sdl_init (sat_sdl_t **object, const char *title, uint16_t width, uint16_t height)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl init error");

    if (object != NULL && title != NULL && width > 0 && height > 0)
    {

        sat_sdl_t *__object = NULL;
        do 
        {
            
            status = sat_sdl_init_api ();
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            __object = calloc (1, sizeof (struct sat_sdl_t));
            if (__object == NULL)
            {
                break;
            }

            status = sat_sdl_window_create (&__object->window, title, width, height);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            __object->running = true;
            __object->height = height;
            __object->width = width;
            __object->initialized = true;

            *object = __object;

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

sat_status_t sat_sdl_set_image (sat_sdl_t *object, const char *name)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl set image error");

    if (object != NULL && object->initialized == true && name != NULL)
    {
        for (uint8_t i = 0; i < object->images.amount; i++)
        {
            if (strcmp (object->images.list [i].name, name) == 0)
            {
                sat_sdl_window_set_image (&object->window, object->images.list [i].handle);

                sat_status_set (&status, true, "");

                break;
            }
        }
    }

    return status;
}

sat_status_t sat_sdl_image_add (sat_sdl_t *object, char *name, const char *file, sat_sdl_image_type_t type)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl image add error");

    if (object != NULL && object->initialized == true && name != NULL && file != NULL && object->images.amount < SAT_SDL_IMAGES_SIZE)
    {
        sat_sdl_image_t image = 
        {
            .name = name,
            .type = type
        };

        status = sat_sdl_image_load (&image, file);

        if (sat_status_get_result (&status) == true)
        {
            memcpy (&object->images.list [object->images.amount], &image, sizeof (sat_sdl_image_t));

            object->images.amount ++;
        }
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

        for (uint8_t i = 0; i < object->images.amount; i++)
        {
            sat_sdl_image_unload (&object->images.list [i]);
        }

        free (object);

        sat_sdl_close_api ();

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_sdl_init_api (void)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl init api error");

    int image_flag = IMG_INIT_PNG;

    if (SDL_Init (SDL_INIT_EVERYTHING) >= 0 &&
        (IMG_Init (image_flag) & image_flag) != 0)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static void sat_sdl_close_api (void)
{
    SDL_Quit ();
    IMG_Quit ();
}