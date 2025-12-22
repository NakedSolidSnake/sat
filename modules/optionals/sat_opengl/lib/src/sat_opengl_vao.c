#include <sat_opengl_vao.h>
#include <GL/glew.h>

sat_status_t sat_opengl_vao_create (sat_opengl_vao_t *object, const char *name)
{
    glGenVertexArrays (1, &object->id);
    object->name = name;

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}

sat_status_t sat_opengl_vao_destroy (sat_opengl_vao_t *object)
{
    glDeleteVertexArrays (1, &object->id);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}

sat_status_t sat_opengl_vao_enable (sat_opengl_vao_t *object)
{
    glBindVertexArray (object->id);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}

sat_status_t sat_opengl_vao_disable (sat_opengl_vao_t *object)
{
    (void) object;

    glBindVertexArray (0);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}