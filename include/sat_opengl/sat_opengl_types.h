#ifndef SAT_OPENGL_TYPES_H_
#define SAT_OPENGL_TYPES_H_

#include <stdint.h>
#include <stdbool.h>
#include <cglm/cglm.h>

typedef enum 
{
    sat_opengl_shader_type_vertex,
    sat_opengl_shader_type_fragment,
    
} sat_opengl_shader_type_t;

typedef struct 
{
    float *list;
    uint32_t size;

} sat_opengl_vertices_t;

typedef struct 
{
    unsigned int *list;
    uint32_t size;

} sat_opengl_indexes_t;

typedef struct 
{
    int location;
    int amount;
    int elements;
    int offset;

} sat_opengl_attribute_t;

typedef struct 
{
    sat_opengl_attribute_t *list;
    uint8_t amount;

} sat_opengl_attributes_t;

typedef struct 
{
    const char *name;
    sat_opengl_vertices_t vertices;
    sat_opengl_attributes_t attributes;
    sat_opengl_indexes_t indexes;

} sat_opengl_vbo_args_t;

typedef struct 
{
    float red;
    float green;
    float blue;
    float alpha;
    
} sat_opengl_color_t;

typedef enum 
{
    sat_opengl_draw_type_triangles,
    sat_opengl_draw_type_elements,
    
} sat_opengl_draw_type_t;

typedef enum 
{
    sat_opengl_value_send_one,
    sat_opengl_value_send_two,
    sat_opengl_value_send_three,
    sat_opengl_value_send_four,

} sat_opengl_value_send_t;

typedef struct 
{
    float first;
    float second;
    float third;
    float fourth;

} sat_opengl_value_float_t;

typedef struct 
{
    int first;
    int second;
    int third;
    int fourth;
    
} sat_opengl_value_int_t;

typedef struct 
{
    bool first;
    bool second;
    bool third;
    bool fourth;
    
} sat_opengl_value_bool_t;

typedef enum
{
    sat_opengl_value_type_bool,
    sat_opengl_value_type_int,
    sat_opengl_value_type_float,

} sat_opengl_value_type_t;

typedef struct 
{
   sat_opengl_value_type_t type;
   sat_opengl_value_send_t send;
   sat_opengl_value_bool_t bools;
   sat_opengl_value_int_t ints;
   sat_opengl_value_float_t floats;

} sat_opengl_value_t;


typedef enum
{
    sat_opengl_texture_format_jpg,
    sat_opengl_texture_format_png

} sat_opengl_texture_format_t;

typedef enum
{
    sat_opengl_texture_type_2D,
    sat_opengl_texture_type_3D

} sat_opengl_texture_type_t;

typedef struct 
{
    sat_opengl_texture_type_t type;
    sat_opengl_texture_format_t format;
    const char *filename;
    bool flip;

} sat_opengl_texture_args_t;

typedef enum 
{
    sat_opengl_matrix_type_2x2,
    sat_opengl_matrix_type_3x3,
    sat_opengl_matrix_type_4x4,

} sat_opengl_matrix_type_t;

typedef struct 
{
    sat_opengl_matrix_type_t type;
    mat2 *matrix_2x2;
    mat3 *matrix_3x3;
    mat4 *matrix_4x4;

} sat_opengl_matrix_t;

#endif/* SAT_OPENGL_TYPES_H_ */
