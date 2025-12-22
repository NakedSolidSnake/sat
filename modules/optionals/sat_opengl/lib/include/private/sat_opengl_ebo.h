#ifndef SAT_OPENGL_EBO_H_
#define SAT_OPENGL_EBO_H_

#include <sat_status.h>
#include <sat_opengl_types.h>

typedef struct 
{
    unsigned int id;

} sat_opengl_ebo_t;

sat_status_t sat_opengl_ebo_create (sat_opengl_ebo_t *object);
sat_status_t sat_opengl_ebo_set_indexes (sat_opengl_ebo_t *object, sat_opengl_indexes_t *indexes);
sat_status_t sat_opengl_ebo_enable (sat_opengl_ebo_t *object);

#endif/* SAT_OPENGL_EBO_H_ */
