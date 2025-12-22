#include <sat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define SAT_SDL_SCREEN_WIDTH        480
#define SAT_SDL_SCREEN_HEIGHT       700

typedef struct 
{
    sat_sdl_t *sdl;
    sat_sdl_coordinate_t position;
    sat_sdl_flip_t flip;
} game_t;

static void sat_sdl_event_on_key_pressed (void *object, sat_sdl_key_t key)
{
    game_t *game = (game_t *)object;

    char *move = "idle";

    if (key == sat_sdl_key_right)
    {
        game->position.x += 5;
        move = "run";
        game->flip = sat_sdl_flip_none;
    }

    else if (key == sat_sdl_key_left)
    {
        game->position.x -= 5;
        move = "run";
        game->flip = sat_sdl_flip_horizontal;
    }

    else if (key == sat_sdl_key_down)
    {
        if (game->flip == sat_sdl_flip_none)
            game->position.x += 10;
        
        else 
            game->position.x -= 10;

        move = "slide";
    }

    sat_status_t status = sat_sdl_clear (game->sdl);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_animate_draw (game->sdl, "background", NULL, (sat_sdl_coordinate_t){}, sat_sdl_flip_none);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_animate_draw (game->sdl, "hero", move, game->position, game->flip);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_draw (game->sdl);
    assert (sat_status_get_result (&status) == true);
}

static void load_animation (sat_sdl_t *sdl, char *name, char *path, char *filename, bool type)
{
    char buffer [1024] = {0};
    snprintf (buffer, 1024 - 1, "%s/%s", path, filename);

    sat_sdl_animate_properties_t properties = 
    {
        .image_type = sat_sdl_image_type_png,
        .type = type == true ? sat_sdl_animate_type_background : sat_sdl_animate_type_sprite,
        .orientation = sat_sdl_background_orientation_horizontal,
        .sprites.dimension = 
        {
            .width = 50,
            .height = 37
        }
    };

    sat_status_t status = sat_sdl_animate_add (sdl, buffer, name, properties);
    assert (sat_status_get_result (&status) == true);
}


int main (int argc, char *argv[])
{
    game_t game;

    memset (&game, 0, sizeof (game_t));

    sat_status_t status = sat_sdl_init (&game.sdl, "Window Title", SAT_SDL_SCREEN_WIDTH, SAT_SDL_SCREEN_HEIGHT);
    assert (sat_status_get_result (&status) == true);

    load_animation (game.sdl, "background", argv[1], "space.png", true);
    load_animation (game.sdl, "hero", argv[1], "hero.png", false);

    sat_sdl_frame_position_t idle [] = 
    {
        {.row = 0, .column = 0},
        {.row = 0, .column = 1},
        {.row = 0, .column = 2},
        {.row = 0, .column = 3},
    };

    sat_sdl_animate_add_states (game.sdl, "hero", "idle", idle, sizeof (idle) / sizeof (idle [0]));
    
    sat_sdl_frame_position_t attack [] = 
    {
        {.row = 6, .column = 0},
        {.row = 6, .column = 1},
        {.row = 6, .column = 2},
        {.row = 6, .column = 3},
        {.row = 6, .column = 4},
    };

    sat_sdl_animate_add_states (game.sdl, "hero", "attack", attack, sizeof (attack) / sizeof (attack [0]));

    sat_sdl_frame_position_t run [] = 
    {
        {.row = 1, .column = 1},
        {.row = 1, .column = 2},
        {.row = 1, .column = 3},
        {.row = 1, .column = 4},
        {.row = 1, .column = 5},
        {.row = 1, .column = 6},
    };

    sat_sdl_animate_add_states (game.sdl, "hero", "run", run, sizeof (run) / sizeof (run [0]));

    sat_sdl_frame_position_t slide [] = 
    {
        {.row = 3, .column = 3},
        {.row = 3, .column = 4},
        {.row = 3, .column = 5},
        {.row = 3, .column = 6},
        {.row = 4, .column = 0},
    };

    sat_sdl_animate_add_states (game.sdl, "hero", "slide", slide, sizeof (slide) / sizeof (slide [0]));

    status = sat_sdl_set_event_key_pressed (game.sdl, sat_sdl_event_on_key_pressed);
    assert (sat_status_get_result (&status) == true);

    status = sat_sdl_set_context (game.sdl, &game);
    assert (sat_status_get_result (&status) == true);

    do 
    {
        status = sat_sdl_scan_events (game.sdl);

        usleep (100000);
    } while (sat_status_get_result (&status) == true);
    

    status = sat_sdl_close (game.sdl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}