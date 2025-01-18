#include <sat_sdl.h>
#include <string.h>
#include <stdlib.h>
#include <sat_sdl_window.h>
#include <sat_sdl_render.h>
#include <sat_sdl_image.h>
#include <sat_sdl_texture.h>
#include <sat_sdl_geometry.h>
#include <sat_sdl_mouse.h>
#include <sat_sdl_font.h>
#include <sat_sdl_sound.h>
#include <sat_sdl_animate.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define SAT_SDL_TEXTURES_SIZE     20
#define SAT_SDL_FONTS_AMOUNT      20
#define SAT_SDL_SOUND_AMOUNT      20
#define SAT_SDL_ANIMATE_AMOUNT    20

struct sat_sdl_t
{
    sat_sdl_window_t window;
    sat_sdl_render_t render;
    uint16_t width;
    uint16_t height;
    bool running;
    bool initialized;

    struct 
    {
        sat_sdl_event_on_key_pressed_t on_key_pressed;
        sat_sdl_event_on_key_released_t on_key_released;
        sat_sdl_event_on_mouse_event_t on_mouse_event;
    } events;

    struct 
    {
        sat_sdl_texture_t list [SAT_SDL_TEXTURES_SIZE];
        uint8_t amount;
    } textures;

    struct
    {
        sat_sdl_font_t list [SAT_SDL_FONTS_AMOUNT];
        uint8_t amount;
    } fonts;

    struct
    {
        sat_sdl_sound_t list [SAT_SDL_SOUND_AMOUNT];
        uint8_t amount;
    } sounds;

    struct 
    {
        sat_sdl_animate_t list [SAT_SDL_ANIMATE_AMOUNT];
        uint8_t amount;
    } animates;

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

            status = sat_sdl_render_create (&__object->render, &__object->window);
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
        sat_sdl_render_set_color_background (&object->render, color);

        sat_status_set (&status, true, "");
    }

    return status; 
}

sat_status_t sat_sdl_set_image (sat_sdl_t *object, const char *name, sat_sdl_rectangle_t rectangle)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl set image error");

    if (object != NULL && object->initialized == true && name != NULL)
    {
        for (uint8_t i = 0; i < object->textures.amount; i++)
        {
            if (strcmp (object->textures.list [i].name, name) == 0)
            {
                sat_sdl_render_set_texture (&object->render, object->textures.list [i].handle, rectangle);

                sat_status_set (&status, true, "");

                break;
            }
        }
    }

    return status;
}

sat_status_t sat_sdl_get_image_rectangle_in_position (sat_sdl_t *object, const char *name, sat_sdl_coordinate_t coordinate, sat_sdl_rectangle_t *rectangle)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl get image rectangle in position error");

    if (object != NULL && object->initialized == true && name != NULL && rectangle != NULL)
    {
        for (uint8_t i = 0; i < object->textures.amount; i++)
        {
            if (strcmp (object->textures.list [i].name, name) == 0)
            {
                rectangle->coordinate = coordinate;

                sat_sdl_texture_get_dimension (&object->textures.list [i], &rectangle->dimension);

                // sat_sdl_render_set_texture (&object->render, object->textures.list [i].handle, rectangle);

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

    if (object != NULL && object->initialized == true && name != NULL && file != NULL && object->textures.amount < SAT_SDL_TEXTURES_SIZE)
    {
        sat_sdl_image_t image = 
        {
            .type = type
        };

        status = sat_sdl_image_load (&image, file);

        if (sat_status_get_result (&status) == true)
        {
            sat_sdl_texture_t texture = 
            {
                .name = name
            };

            sat_sdl_texture_create (&texture, &object->render, &image);

            memcpy (&object->textures.list [object->textures.amount], &texture, sizeof (sat_sdl_texture_t));

            sat_sdl_image_unload (&image);

            object->textures.amount ++;
        }
    }

    return status;
}

sat_status_t sat_sdl_font_add (sat_sdl_t *object, char *name, const char *file, uint16_t size)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl font add error");

    if (object != NULL && object->initialized == true && name != NULL && file != NULL && object->fonts.amount < SAT_SDL_FONTS_AMOUNT)
    {
        sat_sdl_font_t font = 
        {
            .name = name,
            .size = size
        };

        status = sat_sdl_font_load (&font, size, file);

        if (sat_status_get_result (&status) == true)
        {

            memcpy (&object->fonts.list [object->fonts.amount], &font, sizeof (sat_sdl_font_t));

            object->fonts.amount ++;
        }
    }

    return status;
}

sat_status_t sat_sdl_set_text (sat_sdl_t *object, char *font, char *text, sat_sdl_rectangle_t rectangle)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl set text error");

    if (object != NULL && object->initialized == true && font != NULL && text != NULL)
    {
        for (uint8_t i = 0; i < object->fonts.amount; i++)
        {
            if (strcmp (object->fonts.list [i].name, font) == 0)
            {

                SDL_Color __color =
                {
                    .r = rectangle.color.red,
                    .g = rectangle.color.green,
                    .b = rectangle.color.blue,
                    .a = rectangle.color.alpha,
                };

                SDL_Surface *surface = TTF_RenderText_Solid (object->fonts.list [i].handle, text, __color);

                SDL_Texture *texture = SDL_CreateTextureFromSurface (object->render.render, surface);

                SDL_FreeSurface (surface);

                sat_sdl_render_set_texture (&object->render, texture, rectangle);

                SDL_DestroyTexture (texture);

                sat_status_set (&status, true, "");

                break;
            }
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

sat_status_t sat_sdl_set_event_key_released (sat_sdl_t *object, sat_sdl_event_on_key_pressed_t on_key_released)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl set event key error");

    if (object != NULL && object->initialized == true && on_key_released != NULL)
    {
        object->events.on_key_released = on_key_released;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_sdl_set_event_mouse_event (sat_sdl_t *object, sat_sdl_event_on_mouse_event_t on_mouse_event)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl set event key error");

    if (object != NULL && object->initialized == true && on_mouse_event != NULL)
    {
        object->events.on_mouse_event = on_mouse_event;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_sdl_wait_key_pressed (sat_sdl_t *object, sat_sdl_key_t *key)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl set context error");

    if (object != NULL && object->initialized == true && key != NULL)
    {
        SDL_Event event;

        while(true)
        {
            SDL_WaitEvent(&event);

            printf ("Event type: %d\n", event.type);
            if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT)
            {
                break;
            }
        }

        *key = sat_sdl_key_get (event.key.keysym.sym);

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

sat_status_t sat_sdl_set_viewport (sat_sdl_t *object, sat_sdl_rectangle_t rectangle)
{
   sat_status_t status = sat_status_set (&status, false, "sat sdl set viewport error");

    if (object != NULL && object->initialized == true)
    {
        sat_sdl_render_set_viewport (&object->render, rectangle);
        
        sat_status_set (&status, true, "");
    }

    return status; 
}

sat_status_t sat_sdl_clear (sat_sdl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl clear error");

    if (object != NULL && object->initialized == true)
    {
        sat_sdl_render_clear (&object->render);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_sdl_draw_point (sat_sdl_t *object, sat_sdl_point_t point)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl draw point error");

    if (object != NULL && object->initialized == true)
    {
        sat_sdl_geometry_draw_point (&object->render, point);

        sat_status_set (&status, true, "");
    }

    return status;  
}

sat_status_t sat_sdl_draw_line (sat_sdl_t *object, sat_sdl_line_t line)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl draw line error");

    if (object != NULL && object->initialized == true)
    {
        sat_sdl_geometry_draw_line (&object->render, line);

        sat_status_set (&status, true, "");
    }

    return status; 
}

sat_status_t sat_sdl_draw_rectangle (sat_sdl_t *object, sat_sdl_rectangle_t rectangle)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl draw rectangle error");

    if (object != NULL && object->initialized == true)
    {
        sat_sdl_geometry_draw_rectangle (&object->render, rectangle);

        sat_status_set (&status, true, "");
    }

    return status; 
}

sat_status_t sat_sdl_draw_circle (sat_sdl_t *object, sat_sdl_circle_t circle)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl draw circle error");

    if (object != NULL && object->initialized == true)
    {
        sat_sdl_geometry_draw_circle (&object->render, circle);

        sat_status_set (&status, true, "");
    }

    return status; 
}

sat_status_t sat_sdl_draw (sat_sdl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl draw error");

    if (object != NULL && object->initialized == true)
    {
        sat_sdl_render_draw (&object->render);

        sat_status_set (&status, true, "");
    }

    return status;    
}

sat_status_t sat_sdl_scan_events (sat_sdl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl scan events error");
    SDL_Event event;

    if (object != NULL && object->initialized == true)
    {
        sat_status_set (&status, true, "");

        // const Uint8 *key_state = SDL_GetKeyboardState (NULL);

        // if (object->events.on_key_pressed != NULL)
        // {
        //     object->events.on_key_pressed (object->context,
        //                                     sat_sdl_key_get_by (key_state));
        // }
        
        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_QUIT || object->running == false)
            {
                sat_status_set (&status, false, "");
                break;
            }

            else if (event.type == SDL_KEYDOWN && object->events.on_key_pressed != NULL)
            {
                object->events.on_key_pressed (object->context,
                                            sat_sdl_key_get (event.key.keysym.sym));
            }

            else if (event.type == SDL_KEYUP && object->events.on_key_released != NULL)
            {
                object->events.on_key_released (object->context,
                                            sat_sdl_key_get (event.key.keysym.sym));
            }

            else if ((event.type == SDL_MOUSEMOTION     ||
                        event.type == SDL_MOUSEBUTTONDOWN ||
                        event.type == SDL_MOUSEBUTTONUP)  &&
                        object->events.on_mouse_event != NULL)
            {
                sat_sdl_coordinate_t coordinate;
                SDL_GetMouseState (&coordinate.x, &coordinate.y);

                object->events.on_mouse_event (object->context, 
                                               sat_sdl_mouse_event_get_type (event.type),
                                               coordinate);
            }
        }
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
        sat_sdl_render_destroy (&object->render);
        sat_sdl_window_destroy (&object->window);

        for (uint8_t i = 0; i < object->textures.amount; i++)
        {
            sat_sdl_texture_destroy (&object->textures.list [i]);
        }

        for (uint8_t i = 0; i < object->sounds.amount; i++)
        {
            sat_sdl_sound_destroy (&object->sounds.list [i]);
        }

        free (object);

        sat_sdl_close_api ();

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_sdl_audio_add (sat_sdl_t *object, char *filename, char *name, sat_sdl_audio_type_t type)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl audio add error");

    if (object != NULL && object->initialized == true && name != NULL && filename != NULL && object->sounds.amount < SAT_SDL_SOUND_AMOUNT)
    {
        sat_sdl_sound_t sound;

        status = sat_sdl_sound_load (&sound, filename, name, type);

        if (sat_status_get_result (&status) == true)
        {
            memcpy (&object->sounds.list [object->sounds.amount], &sound, sizeof (sat_sdl_sound_t));

            object->sounds.amount ++;
        }
    }

    return status;
}

sat_status_t sat_sdl_audio_control (sat_sdl_t *object, char *name, sat_sdl_audio_control_t control)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl audio control error");

    if (object != NULL && object->initialized == true && name != NULL)
    {
        for (uint8_t i = 0; i < object->sounds.amount; i++)
        {
            sat_sdl_sound_t *sound = &object->sounds.list [i];

            if (strcmp (sound->name, name) == 0)
            {
                switch (control)
                {
                    case sat_sdl_audio_control_play:

                    if (sound->type == sat_sdl_audio_type_fx)
                        sat_sdl_sound_fx_play (sound);
                    
                    else 
                        sat_sdl_sound_music_play (sound);

                    break;

                    case sat_sdl_audio_control_stop:
                        sat_sdl_sound_music_stop (sound);
                    break;

                    case sat_sdl_audio_control_pause:
                        sat_sdl_sound_music_pause (sound);
                    break;

                    case sat_sdl_audio_control_resume:
                        sat_sdl_sound_music_resume (sound);
                    break;
                }

                sat_status_set (&status, true, "");
            }
        }
    }

    return status;
}

sat_status_t sat_sdl_delay (sat_sdl_t *object, uint32_t delay)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl delay error");

    if (object != NULL && object->initialized == true)
    {
        SDL_Delay (delay);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_sdl_animate_add (sat_sdl_t *object, char *filename, char *name, sat_sdl_animate_properties_t properties)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl animate add error");

    if (object != NULL && object->initialized == true && name != NULL && filename != NULL && object->animates.amount < SAT_SDL_ANIMATE_AMOUNT)
    {
        sat_sdl_image_t image = 
        {
            .type = properties.image_type
        };

        status = sat_sdl_image_load (&image, filename);

        if (sat_status_get_result (&status) == true)
        {
            sat_sdl_animate_t animate = 
            {
                .offset = 0,
                .orientation = properties.orientation,
                .type = properties.type,
                .texture.name = name,
                .dimension = properties.sprites.dimension,
            };

            sat_sdl_animate_load (&animate, &object->render, &image);

            memcpy (&object->animates.list [object->animates.amount], &animate, sizeof (sat_sdl_animate_t));

            sat_sdl_image_unload (&image);

            object->animates.amount ++;
        }
    }

    return status;
}

sat_status_t sat_sdl_animate_add_states (sat_sdl_t *object, char *name, char *state, sat_sdl_frame_position_t *positions, uint8_t size)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl animate add states error");

    if (object != NULL && object->initialized == true && name != NULL && state != NULL && positions != NULL && size > 0)
    {
        for (uint8_t i = 0; i < object->animates.amount; i++)
        {
            sat_sdl_animate_t *animate = &object->animates.list [i];
            
            if (strcmp (animate->texture.name, name) == 0 && animate->type == sat_sdl_animate_type_sprite)
            {
                status = sat_sdl_animate_add_state (animate, state, positions, size);

                break;
            }
        }
    }

    return status;
}

sat_status_t sat_sdl_animate_draw (sat_sdl_t *object, char *name, char *state, sat_sdl_coordinate_t coordinate, sat_sdl_flip_t flip)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl animate draw error");

    if (object != NULL && object->initialized == true && name != NULL)
    {
        for (uint8_t i = 0; i < object->animates.amount; i++)
        {
            sat_sdl_animate_t *animate = &object->animates.list [i];

            if (strcmp (animate->texture.name, name) == 0)
            {
                if (animate->type == sat_sdl_animate_type_background)
                {
                    sat_sdl_rectangle_t rectangle = 
                    {
                        .dimension = 
                        {
                            .height = object->height,
                            .width = object->width
                        }
                    };

                    if (animate->orientation == sat_sdl_background_orientation_horizontal)
                    {
                        if (-- animate->offset < -object->width)
                            animate->offset = 0;

                        rectangle.coordinate.x = animate->offset;
                        sat_sdl_render_set_texture (&object->render, animate->texture.handle, rectangle);

                        rectangle.coordinate.x = animate->offset + object->width;
                        sat_sdl_render_set_texture (&object->render, animate->texture.handle, rectangle);

                    }

                    else
                    {
                        if (-- animate->offset > -object->height)
                            animate->offset = 0;

                        rectangle.coordinate.y = animate->offset;
                        sat_sdl_render_set_texture (&object->render, animate->texture.handle, rectangle);

                        rectangle.coordinate.y = animate->offset + object->height;
                        sat_sdl_render_set_texture (&object->render, animate->texture.handle, rectangle);

                    }
                }

                else 
                {
                    sat_sdl_rectangle_t *rectangle;

                    sat_sdl_animate_get_frame (animate, state, animate->offset, &rectangle);

                    sat_sdl_render_set_texture_xy (&object->render, animate->texture.handle, *rectangle, coordinate, flip);

                    animate->offset ++;

                    if (animate->offset >= animate->frames.amount)
                        animate->offset = 0;
                }

                sat_status_set (&status, true, "");

                break;
            }
        }
    }

    return status;
}

static sat_status_t sat_sdl_init_api (void)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl init api error");

    int image_flag = IMG_INIT_PNG;

    if (SDL_Init (SDL_INIT_EVERYTHING) >= 0 &&
        (IMG_Init (image_flag) & image_flag) != 0 &&
        TTF_Init () >= 0 && 
        Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static void sat_sdl_close_api (void)
{
    TTF_Quit ();
    IMG_Quit ();
    Mix_Quit ();
    SDL_Quit ();
}