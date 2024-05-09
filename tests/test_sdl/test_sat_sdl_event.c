#include <sat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct 
{
    sat_sdl_key_t key;
    bool pressed;
    char *command;
} key_is_pressed_t;

static key_is_pressed_t is_pressed [4] = 
{
    {.key = sat_sdl_key_up,    .pressed = false, .command = "up"},
    {.key = sat_sdl_key_down,  .pressed = false, .command = "down"},
    {.key = sat_sdl_key_left,  .pressed = false, .command = "left"},
    {.key = sat_sdl_key_right, .pressed = false, .command = "right"},
};

static void load_image (sat_sdl_t *sdl, char *name, char *path, char *filename)
{
    char buffer [1024] = {0};
    snprintf (buffer, 1024 - 1, "%s/%s", path, filename);

    sat_status_t status = sat_sdl_image_add (sdl, name, buffer, sat_sdl_image_type_png);
    assert (sat_status_get_result (&status) == true);
}

static void sat_sdl_event_on_key_pressed (void *object, sat_sdl_key_t key)
{
    sat_sdl_t *sdl = (sat_sdl_t *)object;

    printf ("Key Pressed: %d\n", key);

    for (int i = 0; i < 4; i++)
    {
        if (is_pressed [i].key == key)
        {
            sat_sdl_set_image (sdl, is_pressed [i].command);
            is_pressed [i].pressed = true;
            sat_sdl_refresh (sdl);
        }
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

    load_image (sdl, "center", argv[1], "center.png");
    load_image (sdl, "left",   argv[1], "left.png");
    load_image (sdl, "right",  argv[1], "right.png");
    load_image (sdl, "down",   argv[1], "down.png");
    load_image (sdl, "up",     argv[1], "up.png");

    status = sat_sdl_set_image (sdl, "center");
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_refresh (sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_run (sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_close (sdl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}