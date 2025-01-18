#ifndef SAT_OPENGL_VAO_H_
#define SAT_OPENGL_VAO_H_

#include <sat_status.h>

typedef struct 
{
    unsigned int id;
    const char *name;

} sat_opengl_vao_t;

sat_status_t sat_opengl_vao_create (sat_opengl_vao_t *object, const char *name);
sat_status_t sat_opengl_vao_destroy (sat_opengl_vao_t *object);

sat_status_t sat_opengl_vao_enable (sat_opengl_vao_t *object);
sat_status_t sat_opengl_vao_disable (sat_opengl_vao_t *object);

#endif/* SAT_OPENGL_VAO_H_ */
