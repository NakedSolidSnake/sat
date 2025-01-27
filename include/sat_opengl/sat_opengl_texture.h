#ifndef SAT_OPENGL_TEXTURE_H_
#define SAT_OPENGL_TEXTURE_H_

#include <sat_status.h> 
#include <sat_opengl_types.h>

typedef struct 
{
    unsigned int id;
    const char *name;
    sat_opengl_texture_type_t type;

} sat_opengl_texture_t;

sat_status_t sat_opengl_texture_open (sat_opengl_texture_t *object, sat_opengl_texture_args_t *args);
void sat_opengl_texture_enable (sat_opengl_texture_t *object);
void sat_opengl_texture_activate (sat_opengl_texture_t *object, uint16_t position);

#endif/* SAT_OPENGL_TEXTURE_H_ */
