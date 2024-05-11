#include <sat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
    sat_sdl_t *sdl;

    sat_status_t status = sat_sdl_init (&sdl, "Window Title", 640, 480);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_background (sdl, (sat_sdl_color_t){.red = 0xFF, .green = 0xFF, .blue = 0xFF});
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_draw (sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_run (sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_close (sdl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}