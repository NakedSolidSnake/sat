#ifndef SAT_OPENGL_SHADER_H_
#define SAT_OPENGL_SHADER_H_

#include <sat_opengl_types.h>
#include <sat_status.h>

typedef struct 
{
    unsigned int id;
    const char *name;

} sat_opengl_shader_t;

sat_status_t sat_opengl_shader_create_from_file (sat_opengl_shader_t *object, sat_opengl_shader_type_t type, const char *path);
sat_status_t sat_opengl_shader_free (sat_opengl_shader_t *object);

#endif/* SAT_OPENGL_SHADER_H_ */
