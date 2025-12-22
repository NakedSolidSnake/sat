#include <sat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define SAT_SDL_SCREEN_WIDTH        640
#define SAT_SDL_SCREEN_HEIGHT       480

int main (int argc, char *argv[])
{
    sat_sdl_t *sdl;

    sat_status_t status = sat_sdl_init (&sdl, "Window Title", SAT_SDL_SCREEN_WIDTH, SAT_SDL_SCREEN_HEIGHT);
    assert (sat_status_get_result (&status) == true);

    char filename [1024] = {0};
    snprintf (filename, 1024 - 1, "%s/%s", argv [1], "ArianaVioleta-dz2K.ttf");

    status = sat_sdl_font_add (sdl, "ariana", filename, 150);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_text (sdl, "ariana", "Hello World", (sat_sdl_rectangle_t)
                                                             {
                                                                .coordinate = 
                                                                {
                                                                    .x = 0,
                                                                    .y = 0
                                                                },
                                                                .dimension = 
                                                                {
                                                                    .width = SAT_SDL_SCREEN_WIDTH,
                                                                    .height = SAT_SDL_SCREEN_HEIGHT
                                                                },
                                                                .color = 
                                                                {
                                                                    .red = 0XAA,
                                                                    .green = 0XAA,
                                                                    .blue = 0xFF
                                                                }
                                                             });

    assert (sat_status_get_result (&status) == true);

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