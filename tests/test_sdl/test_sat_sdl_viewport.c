#include <sat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define SAT_SDL_SCREEN_WIDTH        640
#define SAT_SDL_SCREEN_HEIGHT       480

static void set_viewport (sat_sdl_t *sdl, char *image, sat_sdl_coordinate_t coordinate);
static void on_mouse_event (void *object, sat_sdl_mouse_event_type_t event, sat_sdl_coordinate_t coordinate);

static void on_mouse_event (void *object, sat_sdl_mouse_event_type_t event, sat_sdl_coordinate_t coordinate)
{
    sat_sdl_t *sdl = (sat_sdl_t *) object;
    char *image = "center";
    
    switch (event)
    {
    case sat_sdl_mouse_event_type_motion:
        break;

    case sat_sdl_mouse_event_type_button_down:
        image = "down";
        break;

    case sat_sdl_mouse_event_type_button_up:
        image = "up";
        break;
    
    default:
        break;
    }


    set_viewport (sdl, image, coordinate);
}

static void set_viewport (sat_sdl_t *sdl, char *image, sat_sdl_coordinate_t coordinate)
{
    sat_status_t status = sat_sdl_set_viewport (sdl, (sat_sdl_rectangle_t)
                                                     {
                                                         .coordinate = 
                                                         {
                                                             .x = coordinate.x - 100,
                                                             .y = coordinate.y,
                                                         },
                                                         .dimension = 
                                                         {
                                                             .width = 100,
                                                             .height = 100,
 
                                                         }
                                                     });
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_image (sdl, image, (sat_sdl_rectangle_t)
                                            {
                                                .coordinate = 
                                                {
                                                    .x = 0,
                                                    .y = 0,
                                                },
                                                .dimension = 
                                                {
                                                    .width = 100,
                                                    .height = 100,

                                                }
                                            });
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_draw (sdl);
    assert (sat_status_get_result (&status) == true);
}

static void load_image (sat_sdl_t *sdl, char *name, char *path, char *filename)
{
    char buffer [1024] = {0};
    snprintf (buffer, 1024 - 1, "%s/%s", path, filename);

    sat_status_t status = sat_sdl_image_add (sdl, name, buffer, sat_sdl_image_type_png);
    assert (sat_status_get_result (&status) == true);
}

int main (int argc, char *argv[])
{
    sat_sdl_t *sdl;

    sat_status_t status = sat_sdl_init (&sdl, "Window Title", SAT_SDL_SCREEN_WIDTH, SAT_SDL_SCREEN_HEIGHT);
    assert (sat_status_get_result (&status) == true);

    load_image (sdl, "center", argv[1], "center.png");
    load_image (sdl, "down",   argv[1], "down.png");
    load_image (sdl, "up",     argv[1], "up.png");

    status = sat_sdl_set_context (sdl, sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_event_mouse_event (sdl, on_mouse_event);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_viewport (sdl, (sat_sdl_rectangle_t)
                                          {
                                                .coordinate = 
                                                {
                                                    .x = 0,
                                                    .y = 0,
                                                },
                                                .dimension = 
                                                {
                                                    .width = SAT_SDL_SCREEN_WIDTH,
                                                    .height = SAT_SDL_SCREEN_HEIGHT,

                                                }
                                          });

    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_image (sdl, "center", (sat_sdl_rectangle_t)
                                               {
                                                    .coordinate = 
                                                    {
                                                        .x = 0,
                                                        .y = 0,
                                                    },
                                                    .dimension = 
                                                    {
                                                        .width = SAT_SDL_SCREEN_WIDTH,
                                                        .height = SAT_SDL_SCREEN_HEIGHT,   
                                                    }
                                               });
    assert (sat_status_get_result (&status) == true);

    set_viewport (sdl, "center", (sat_sdl_coordinate_t)
                                 {
                                    .x = SAT_SDL_SCREEN_WIDTH - 100,
                                    .y = 0,
                                 });

    do 
    {
        status = sat_sdl_draw (sdl);
        assert (sat_status_get_result (&status) == true);

        status = sat_sdl_scan_events (sdl);

        usleep (100000);
    } while (sat_status_get_result (&status) == true);

    status = sat_sdl_close (sdl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}