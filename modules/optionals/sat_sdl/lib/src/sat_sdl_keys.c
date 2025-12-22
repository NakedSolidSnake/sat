#include <sat_sdl_keys.h>
#include <SDL2/SDL.h>

#define SAT_SDL_KEYS_GET_AMOUNT(x) (sizeof (x) / sizeof (x [0]))

#define SAT_SDL_KEYS_PRESSED        1
typedef struct 
{
    SDL_Keycode sdl_key;
    sat_sdl_key_t sat_key;
} sat_sdl_key_map_t;

static const sat_sdl_key_map_t keymap [] = 
{
    {.sdl_key = SDLK_UP,    .sat_key = sat_sdl_key_up   },
    {.sdl_key = SDLK_DOWN,  .sat_key = sat_sdl_key_down },
    {.sdl_key = SDLK_LEFT,  .sat_key = sat_sdl_key_left },
    {.sdl_key = SDLK_RIGHT, .sat_key = sat_sdl_key_right},
    
    {.sdl_key = SDLK_a, .sat_key = sat_sdl_key_A},
    {.sdl_key = SDLK_b, .sat_key = sat_sdl_key_B},
    {.sdl_key = SDLK_c, .sat_key = sat_sdl_key_C},
    {.sdl_key = SDLK_d, .sat_key = sat_sdl_key_D},
    {.sdl_key = SDLK_e, .sat_key = sat_sdl_key_E},
    {.sdl_key = SDLK_f, .sat_key = sat_sdl_key_F},
    {.sdl_key = SDLK_g, .sat_key = sat_sdl_key_G},
    {.sdl_key = SDLK_h, .sat_key = sat_sdl_key_H},
    {.sdl_key = SDLK_i, .sat_key = sat_sdl_key_I},
    {.sdl_key = SDLK_j, .sat_key = sat_sdl_key_J},
    {.sdl_key = SDLK_k, .sat_key = sat_sdl_key_K},
    {.sdl_key = SDLK_l, .sat_key = sat_sdl_key_L},
    {.sdl_key = SDLK_m, .sat_key = sat_sdl_key_M},
    {.sdl_key = SDLK_n, .sat_key = sat_sdl_key_N},
    {.sdl_key = SDLK_o, .sat_key = sat_sdl_key_O},
    {.sdl_key = SDLK_p, .sat_key = sat_sdl_key_P},
    {.sdl_key = SDLK_q, .sat_key = sat_sdl_key_Q},
    {.sdl_key = SDLK_r, .sat_key = sat_sdl_key_R},
    {.sdl_key = SDLK_s, .sat_key = sat_sdl_key_S},
    {.sdl_key = SDLK_t, .sat_key = sat_sdl_key_T},
    {.sdl_key = SDLK_u, .sat_key = sat_sdl_key_U},
    {.sdl_key = SDLK_v, .sat_key = sat_sdl_key_V},
    {.sdl_key = SDLK_x, .sat_key = sat_sdl_key_X},
    {.sdl_key = SDLK_w, .sat_key = sat_sdl_key_W},
    {.sdl_key = SDLK_y, .sat_key = sat_sdl_key_Y},
    {.sdl_key = SDLK_z, .sat_key = sat_sdl_key_Z},
    {.sdl_key = SDLK_0, .sat_key = sat_sdl_key_0},
    {.sdl_key = SDLK_1, .sat_key = sat_sdl_key_1},
    {.sdl_key = SDLK_2, .sat_key = sat_sdl_key_2},
    {.sdl_key = SDLK_3, .sat_key = sat_sdl_key_3},
    {.sdl_key = SDLK_4, .sat_key = sat_sdl_key_4},
    {.sdl_key = SDLK_5, .sat_key = sat_sdl_key_5},
    {.sdl_key = SDLK_6, .sat_key = sat_sdl_key_6},
    {.sdl_key = SDLK_7, .sat_key = sat_sdl_key_7},
    {.sdl_key = SDLK_8, .sat_key = sat_sdl_key_8},
    {.sdl_key = SDLK_9, .sat_key = sat_sdl_key_9},
};

sat_sdl_key_t sat_sdl_key_get_by (const uint8_t *key)
{
    Uint8 *__key = (Uint8 *) key;
    sat_sdl_key_t sat_key = sat_sdl_key_none;

    for (uint16_t i = 0; i < SAT_SDL_KEYS_GET_AMOUNT (keymap); i++)
    {
        if (__key [keymap [i].sdl_key] == SAT_SDL_KEYS_PRESSED)
        {
            sat_key = keymap [i].sat_key;
            break;
        }
    }

    return sat_key;
}

sat_sdl_key_t sat_sdl_key_get (uint32_t key)
{
    sat_sdl_key_t sat_key = sat_sdl_key_none;

    for (uint16_t i = 0; i < SAT_SDL_KEYS_GET_AMOUNT (keymap); i++)
    {
        if (key == keymap [i].sdl_key)
        {
            sat_key = keymap [i].sat_key;
            break;
        }
    }

    return sat_key;
}