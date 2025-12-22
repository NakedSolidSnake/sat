#include <sat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define SAT_SDL_SCREEN_WIDTH        640
#define SAT_SDL_SCREEN_HEIGHT       480

typedef struct 
{
    sat_sdl_key_t key;
    bool pressed;
    char *command;
} key_is_pressed_t;

static sat_sdl_rectangle_t rectangle = 
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
    }
};

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

static void load_audio (sat_sdl_t *sdl, char *name, char *path, char *filename, sat_sdl_audio_type_t type)
{
    char buffer [1024] = {0};
    snprintf (buffer, 1024 - 1, "%s/%s", path, filename);

    sat_status_t status = sat_sdl_audio_add (sdl, buffer, name, type);
    assert (sat_status_get_result (&status) == true);
}

static void sat_sdl_event_on_key_pressed (void *object, sat_sdl_key_t key)
{
    sat_sdl_t *sdl = (sat_sdl_t *)object;

    char *command = "center";

    for (int i = 0; i < 4; i++)
    {
        if (is_pressed [i].key == key)
        {
            printf ("Key Pressed: %d\n", key);
            command = is_pressed [i].command;            
            is_pressed [i].pressed = true;
            
            sat_status_t status = sat_sdl_audio_control (sdl, "hit", sat_sdl_audio_control_play);
            assert (sat_status_get_result (&status) == true);
        }
    }

    sat_sdl_clear (sdl);
    sat_sdl_set_image (sdl, command, rectangle);
    sat_sdl_draw (sdl);

    if (is_pressed [0].pressed == true &&
        is_pressed [1].pressed == true &&
        is_pressed [2].pressed == true &&
        is_pressed [3].pressed == true)
    {
        sat_sdl_stop (sdl);
    }
}

static void sat_sdl_event_on_key_released (void *object, sat_sdl_key_t key)
{
    sat_sdl_t *sdl = (sat_sdl_t *)object;

    char *command = "center";

    sat_sdl_clear (sdl);
    sat_sdl_set_image (sdl, command, rectangle);
    sat_sdl_draw (sdl);
}

int main (int argc, char *argv[])
{
    sat_sdl_t *sdl;
    sat_sdl_key_t key;

    sat_status_t status = sat_sdl_init (&sdl, "Window Title", SAT_SDL_SCREEN_WIDTH, SAT_SDL_SCREEN_HEIGHT);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_event_key_pressed (sdl, sat_sdl_event_on_key_pressed);
    assert (sat_status_get_result (&status) == true);


    status = sat_sdl_set_event_key_released (sdl, sat_sdl_event_on_key_released);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_context (sdl, sdl);
    assert (sat_status_get_result (&status) == true);

    load_image (sdl, "center", argv[1], "center.png");
    load_image (sdl, "left",   argv[1], "left.png");
    load_image (sdl, "right",  argv[1], "right.png");
    load_image (sdl, "down",   argv[1], "down.png");
    load_image (sdl, "up",     argv[1], "up.png");

    load_audio (sdl, "hit", argv [1], "hit_the_ground.wav", sat_sdl_audio_type_fx);
    load_audio (sdl, "game_over", argv [1], "game_over.mp3", sat_sdl_audio_type_music);

    status = sat_sdl_audio_control (sdl, "game_over", sat_sdl_audio_control_play);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_image (sdl, "center", rectangle);
    assert (sat_status_get_result (&status) == true);

    do 
    {
        status = sat_sdl_draw (sdl);
        assert (sat_status_get_result (&status) == true);

        status = sat_sdl_scan_events (sdl);

        usleep (100000);
    } while (sat_status_get_result (&status) == true);

    puts ("Press any key to finish...");
    sat_sdl_wait_key_pressed (sdl, &key);

    printf ("Key: %d\n", key);

    status = sat_sdl_close (sdl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}