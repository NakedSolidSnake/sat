#include <sat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct 
{
    sat_sdl_key_t key;
    bool pressed;
} key_is_pressed_t;

static key_is_pressed_t is_pressed [4] = 
{
    {.key = sat_sdl_key_up,    .pressed = false},
    {.key = sat_sdl_key_down,  .pressed = false},
    {.key = sat_sdl_key_left,  .pressed = false},
    {.key = sat_sdl_key_right, .pressed = false},
};

static void sat_sdl_event_on_key_pressed (void *object, sat_sdl_key_t key)
{
    sat_sdl_t *sdl = (sat_sdl_t *)object;

    printf ("Key Pressed: %d\n", key);

    for (int i = 0; i < 4; i++)
    {
        if (is_pressed [i].key == key)
            is_pressed [i].pressed = true;
    }

    if (is_pressed [0].pressed == true &&
        is_pressed [1].pressed == true &&
        is_pressed [2].pressed == true &&
        is_pressed [3].pressed == true)
    {
        sat_sdl_stop (sdl);
    }
}

int main (int argc, char *argv[])
{
    sat_sdl_t *sdl;

    sat_status_t status = sat_sdl_init (&sdl, "Window Title", 640, 480);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_event_key_pressed (sdl, sat_sdl_event_on_key_pressed);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_context (sdl, sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_background (sdl, (sat_sdl_color_t){.red = 0xFF, .green = 0xFF, .blue = 0xFF});
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_refresh (sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_run (sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_close (sdl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}