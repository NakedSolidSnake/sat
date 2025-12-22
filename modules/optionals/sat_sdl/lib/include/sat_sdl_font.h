#ifndef SAT_SDL_FONT_H_
#define SAT_SDL_FONT_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct 
{
    char *name;
    void *handle;
    uint16_t size;
} sat_sdl_font_t;

sat_status_t sat_sdl_font_load (sat_sdl_font_t *object, uint16_t size, const char *filename);
void sat_sdl_font_destroy (sat_sdl_font_t *object);

#endif/* SAT_SDL_FONT_H_ */
