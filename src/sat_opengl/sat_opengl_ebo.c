#include <sat_opengl_ebo.h>
#include <GL/glew.h>

sat_status_t sat_opengl_ebo_create (sat_opengl_ebo_t *object)
{
    glGenBuffers (1, &object->id);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}

sat_status_t sat_opengl_ebo_set_indexes (sat_opengl_ebo_t *object, sat_opengl_indexes_t *indexes)
{
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, indexes->size, indexes->list, GL_STATIC_DRAW);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}

sat_status_t sat_opengl_ebo_enable (sat_opengl_ebo_t *object)
{
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, object->id);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}