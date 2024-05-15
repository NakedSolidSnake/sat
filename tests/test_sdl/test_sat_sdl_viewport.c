#include <sat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define SAT_SDL_SCREEN_WIDTH        640
#define SAT_SDL_SCREEN_HEIGHT       480

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
    load_image (sdl, "up",   argv[1], "up.png");

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

    status = sat_sdl_set_image (sdl, "center");
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_viewport (sdl, (sat_sdl_rectangle_t)
                                        {
                                            .coordinate = 
                                            {
                                                .x = SAT_SDL_SCREEN_WIDTH - 100,
                                                .y = 0,
                                            },
                                            .dimension = 
                                            {
                                                .width = 100,
                                                .height = 100,

                                            }
                                        });
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_image (sdl, "up");
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_draw (sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_run (sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_close (sdl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}