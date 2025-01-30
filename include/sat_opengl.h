#ifndef SAT_OPENGL_H_
#define SAT_OPENGL_H_

#include <sat_status.h>
#include <stdint.h>

#include <sat_opengl_types.h>

typedef struct sat_opengl_t sat_opengl_t;

typedef struct 
{
    struct 
    {
        uint16_t width;
        uint16_t height;
        const char *title;
    } window;

} sat_opengl_args_t;

sat_status_t sat_opengl_create (sat_opengl_t **object, sat_opengl_args_t *args);
sat_status_t sat_opengl_run (sat_opengl_t *object);
sat_status_t sat_opengl_create_program (sat_opengl_t *object, const char *name);
sat_status_t sat_opengl_add_shader_to_program (sat_opengl_t *object, const char *name, sat_opengl_shader_type_t type, const char *filename);
sat_status_t sat_opengl_compile_program (sat_opengl_t *object, const char *name);
sat_status_t sat_opengl_close (sat_opengl_t *object);

sat_status_t sat_opengl_create_vao (sat_opengl_t *object, const char *name);
sat_status_t sat_opengl_enable_vao (sat_opengl_t *object, const char *name);
sat_status_t sat_opengl_enable_program (sat_opengl_t *object, const char *name);
sat_status_t sat_opengl_draw (sat_opengl_t *object, sat_opengl_draw_type_t type, uint32_t vertices_amount);
sat_status_t sat_opengl_set_color (sat_opengl_t *object, sat_opengl_color_t color);
sat_status_t sat_opengl_add_vbo_to_vao (sat_opengl_t *object, const char *name, sat_opengl_vbo_args_t *args);

sat_status_t sat_opengl_texture_container_create (sat_opengl_t *object, const char *name);
sat_status_t sat_opengl_texture_container_add (sat_opengl_t *object, const char *name, sat_opengl_texture_args_t *args);
sat_status_t sat_opengl_texture_container_enable (sat_opengl_t *object, const char *name);

sat_status_t sat_opengl_send_shader_value (sat_opengl_t *object, const char *name, const char *param, const sat_opengl_value_t *value);
sat_status_t sat_opengl_send_shader_matrix (sat_opengl_t *object, const char *name, const char *param, const sat_opengl_matrix_t *matrix);
sat_status_t sat_opengl_get_time (sat_opengl_t *object, float *value);

sat_status_t sat_opengl_get_window_dimension (sat_opengl_t *object, sat_opengl_dimension_t *dimension);

#endif/* SAT_OPENGL_H_ */
