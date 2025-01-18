#ifndef SAT_SDL_KEYS_H_
#define SAT_SDL_KEYS_H_

#include <stdint.h>

typedef enum 
{
    sat_sdl_key_none,
    sat_sdl_key_up,
    sat_sdl_key_down,
    sat_sdl_key_left,
    sat_sdl_key_right,
    sat_sdl_key_A,
    sat_sdl_key_B,
    sat_sdl_key_C,
    sat_sdl_key_D,
    sat_sdl_key_E,
    sat_sdl_key_F,
    sat_sdl_key_G,
    sat_sdl_key_H,
    sat_sdl_key_I,
    sat_sdl_key_J,
    sat_sdl_key_K,
    sat_sdl_key_L,
    sat_sdl_key_M,
    sat_sdl_key_N,
    sat_sdl_key_O,
    sat_sdl_key_P,
    sat_sdl_key_Q,
    sat_sdl_key_R,
    sat_sdl_key_S,
    sat_sdl_key_T,
    sat_sdl_key_U,
    sat_sdl_key_V,
    sat_sdl_key_X,
    sat_sdl_key_W,
    sat_sdl_key_Y,
    sat_sdl_key_Z,
    sat_sdl_key_0,
    sat_sdl_key_1,
    sat_sdl_key_2,
    sat_sdl_key_3,
    sat_sdl_key_4,
    sat_sdl_key_5,
    sat_sdl_key_6,
    sat_sdl_key_7,
    sat_sdl_key_8,
    sat_sdl_key_9,

} sat_sdl_key_t;

sat_sdl_key_t sat_sdl_key_get_by (const uint8_t *key); 
sat_sdl_key_t sat_sdl_key_get (uint32_t key);

#endif/* SAT_SDL_KEYS_H_ */
