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

    status = sat_sdl_set_background (sdl, (sat_sdl_color_t){.red = 0xFF, .green = 0xFF, .blue = 0xFF});
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_draw_rectangle (sdl, (sat_sdl_rectangle_t)
                                          {
                                            .coordinate = 
                                            {
                                                .x = SAT_SDL_SCREEN_WIDTH / 4,
                                                .y = SAT_SDL_SCREEN_HEIGHT / 4,
                                            },
                                            .dimension = 
                                            {
                                                .width = SAT_SDL_SCREEN_WIDTH / 2,
                                                .height = SAT_SDL_SCREEN_HEIGHT / 2,

                                            },
                                            .color = 
                                            {
                                                .red = 0xFF,
                                                .alpha = 0xFF
                                            },
                                            .fill = true
                                          });

    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_draw_rectangle (sdl, (sat_sdl_rectangle_t)
                                          {
                                            .coordinate = 
                                            {
                                                .x = 10,
                                                .y = 10,
                                            },
                                            .dimension = 
                                            {
                                                .width = SAT_SDL_SCREEN_WIDTH - 20,
                                                .height = SAT_SDL_SCREEN_HEIGHT - 20,

                                            },
                                            .color = 
                                            {
                                                .blue = 0xFF,
                                                .alpha = 0xFF
                                            },
                                            .fill = false
                                          });
                                          
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_draw_line (sdl, (sat_sdl_line_t)
                                     {
                                        .start = 
                                        {
                                            .x = 0,
                                            .y = 0,
                                        },
                                        .end = 
                                        {
                                            .x = SAT_SDL_SCREEN_WIDTH,
                                            .y = SAT_SDL_SCREEN_HEIGHT,
    
                                        },
                                        .color = 
                                        {
                                            .blue = 0xFF,
                                            .alpha = 0xFF
                                        },
                                        .width = 10
                                     });
                                          
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_draw_circle (sdl, (sat_sdl_circle_t)
                                     {
                                        .coordinate = 
                                        {
                                            .x = SAT_SDL_SCREEN_WIDTH / 2,
                                            .y = SAT_SDL_SCREEN_HEIGHT / 2,
    
                                        },
                                        .radius = SAT_SDL_SCREEN_HEIGHT / 4,
                                        .color = 
                                        {
                                            .red = 0x00,
                                            .green = 0x00,
                                            .blue = 0x00,
                                            .alpha = 0xFF
                                        }
                                     });
                                          
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_draw_circle (sdl, (sat_sdl_circle_t)
                                     {
                                        .coordinate = 
                                        {
                                            .x = SAT_SDL_SCREEN_WIDTH / 2,
                                            .y = SAT_SDL_SCREEN_HEIGHT / 2,
    
                                        },
                                        .radius = SAT_SDL_SCREEN_HEIGHT / 8,
                                        .color = 
                                        {
                                            .red = 0x00,
                                            .green = 0x00,
                                            .blue = 0x00,
                                            .alpha = 0xFF
                                        },
                                        .fill = true,
                                     });
                                          
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_draw_point (sdl, (sat_sdl_point_t)
                                     {
                                        .coordinate = 
                                        {
                                            .x = SAT_SDL_SCREEN_WIDTH / 2,
                                            .y = SAT_SDL_SCREEN_HEIGHT / 2,
    
                                        },
                                        .color = 
                                        {
                                            .red = 0xFF,
                                            .green = 0xFF,
                                            .blue = 0xFF,
                                            .alpha = 0xFF
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