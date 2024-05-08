#include <sat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    sat_sdl_t *sdl;

    char filename [1024] = {0};

    sat_status_t status = sat_sdl_init (&sdl, "Window Title", 640, 480);
    assert (sat_status_get_result (&status) == true);

    snprintf (filename, 1024 - 1, "%s/%s", argv [1], "hello.bmp");
    
    status = sat_sdl_image_add (sdl, "hello", filename, sat_sdl_image_type_bmp);
    assert (sat_status_get_result (&status) == true);

    memset (filename, 0, sizeof (filename));
    snprintf (filename, 1024 - 1, "%s/%s", argv [1], "test.png");

    status = sat_sdl_image_add (sdl, "test", filename, sat_sdl_image_type_png);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_image (sdl, "hello");
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_refresh (sdl);
    assert (sat_status_get_result (&status) == true);

    sleep (5);

    status = sat_sdl_set_image (sdl, "test");
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_refresh (sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_run (sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_close (sdl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}