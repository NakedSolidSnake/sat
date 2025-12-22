#include <sat_sdl_sound.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>

sat_status_t sat_sdl_sound_load (sat_sdl_sound_t *object, const char *filename, char *name, sat_sdl_audio_type_t type)
{
    sat_status_t status = sat_status_set (&status, false, "sat sdl sound load error");

    if (object != NULL && filename != NULL)
    {
        if (type == sat_sdl_audio_type_music)
        {
            object->handle = Mix_LoadMUS (filename);
        }

        else 
        {
            object->handle = Mix_LoadWAV (filename);
        }

        if (object->handle != NULL)
        {
            object->name = name;
            object->type = type;

            sat_status_set (&status, true, "");
        }

    }

    return status;
}

void sat_sdl_sound_fx_play (sat_sdl_sound_t *object)
{
    if (object->type == sat_sdl_audio_type_fx)
        Mix_PlayChannel (-1, object->handle, 0);
}

void sat_sdl_sound_music_play (sat_sdl_sound_t *object)
{
    if (object->type == sat_sdl_audio_type_music &&
        sat_sdl_sound_music_is_paused (object) == false && 
        sat_sdl_sound_music_is_playing (object) == false)
    {
        Mix_PlayMusic (object->handle, -1);
    }
}

void sat_sdl_sound_music_resume (sat_sdl_sound_t *object)
{
    if (object->type == sat_sdl_audio_type_music && 
        sat_sdl_sound_music_is_paused (object) == true)
    {
        Mix_ResumeMusic ();
    }
}

void sat_sdl_sound_music_pause (sat_sdl_sound_t *object)
{
    if (object->type == sat_sdl_audio_type_music && 
        sat_sdl_sound_music_is_playing (object) == true)
    {
        Mix_PauseMusic ();
    }
}

void sat_sdl_sound_music_stop (sat_sdl_sound_t *object)
{
    if (object->type == sat_sdl_audio_type_music && 
        sat_sdl_sound_music_is_playing (object) == true)
    {
        Mix_HaltMusic ();
    }
}

bool sat_sdl_sound_music_is_paused (sat_sdl_sound_t *object)
{
    return Mix_PausedMusic () == true;
}

bool sat_sdl_sound_music_is_playing (sat_sdl_sound_t *object)
{
    return Mix_PlayingMusic () == true;
}

void sat_sdl_sound_destroy (sat_sdl_sound_t *object)
{
    if (object != NULL && object->handle != NULL)
    {
        if (object->type == sat_sdl_audio_type_fx)
            Mix_FreeChunk (object->handle);
    }

    else 
    {
        Mix_FreeMusic (object->handle);
    }
}