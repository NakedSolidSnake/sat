#ifndef SAT_SDL_SOUND_H_
#define SAT_SDL_SOUND_H_

#include <sat_status.h>
#include <sat_sdl_types.h>

typedef struct 
{
    char *name;
    void *handle;
    sat_sdl_audio_type_t type;
} sat_sdl_sound_t;

sat_status_t sat_sdl_sound_load (sat_sdl_sound_t *object, const char *filename, char *name, sat_sdl_audio_type_t type);
void sat_sdl_sound_fx_play (sat_sdl_sound_t *object);
void sat_sdl_sound_music_play (sat_sdl_sound_t *object);
void sat_sdl_sound_music_resume (sat_sdl_sound_t *object);
void sat_sdl_sound_music_pause (sat_sdl_sound_t *object);
void sat_sdl_sound_music_stop (sat_sdl_sound_t *object);
bool sat_sdl_sound_music_is_paused (sat_sdl_sound_t *object);
bool sat_sdl_sound_music_is_playing (sat_sdl_sound_t *object);
void sat_sdl_sound_destroy (sat_sdl_sound_t *object);

#endif/* SAT_SDL_SOUND_H_ */
