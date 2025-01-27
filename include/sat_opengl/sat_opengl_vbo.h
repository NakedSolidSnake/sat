#ifndef SAT_OPENGL_VBO_H_
#define SAT_OPENGL_VBO_H_

#include <sat_status.h>
#include <sat_opengl_types.h>

typedef struct 
{
    unsigned int id;
    const char *name;

} sat_opengl_vbo_t;

sat_status_t sat_opengl_vbo_create (sat_opengl_vbo_t *object, const char *name);
sat_status_t sat_opengl_vbo_destroy (sat_opengl_vbo_t *object);

sat_status_t sat_opengl_vbo_set_vertices (sat_opengl_vbo_t *object, sat_opengl_vertices_t *vertices);
sat_status_t sat_opengl_vbo_set_attributes (sat_opengl_vbo_t *object, sat_opengl_attributes_t *attributes);

sat_status_t sat_opengl_vbo_enable (sat_opengl_vbo_t *object);
sat_status_t sat_opengl_vbo_disable (sat_opengl_vbo_t *object);

#endif/* SAT_OPENGL_VBO_H_ */
