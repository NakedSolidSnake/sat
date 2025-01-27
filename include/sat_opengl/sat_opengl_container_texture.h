#ifndef SAT_OPENGL_CONTAINER_TEXTURE_H_
#define SAT_OPENGL_CONTAINER_TEXTURE_H_


#include <sat_opengl_texture.h>

#define SAT_OPENGL_CONTAINER_TEXTURE_AMOUNT     16

typedef struct 
{
    const char *name;
    sat_opengl_texture_t list [SAT_OPENGL_CONTAINER_TEXTURE_AMOUNT];
    uint8_t amount;

} sat_opengl_container_texture_t;

void sat_opengl_container_texture_init (sat_opengl_container_texture_t *object, const char *name);
sat_status_t sat_opengl_container_texture_add (sat_opengl_container_texture_t *object, sat_opengl_texture_t *texture);
void sat_opengl_container_texture_enable (sat_opengl_container_texture_t *object);

#endif/* SAT_OPENGL_CONTAINER_TEXTURE_H_ */
