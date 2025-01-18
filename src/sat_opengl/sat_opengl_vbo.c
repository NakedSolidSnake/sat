#include <sat_opengl_vbo.h>
#include <GL/glew.h>

sat_status_t sat_opengl_vbo_create (sat_opengl_vbo_t *object, const char *name)
{
    glGenBuffers (1, &object->id);
    object->name = name;

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}

sat_status_t sat_opengl_vbo_destroy (sat_opengl_vbo_t *object)
{
    glDeleteBuffers (1, &object->id);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}

sat_status_t sat_opengl_vbo_set_vertices (sat_opengl_vbo_t *object, sat_opengl_vertices_t *vertices)
{
    glBufferData (GL_ARRAY_BUFFER, vertices->size, vertices->list, GL_STATIC_DRAW);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}

sat_status_t sat_opengl_vbo_set_attributes (sat_opengl_vbo_t *object, sat_opengl_attribute_t *attribute)
{
    glVertexAttribPointer (attribute->location,
                           attribute->amount,
                           GL_FLOAT,
                           GL_FALSE,
                           attribute->elements * sizeof (float),
                           (void *)(attribute->offset * sizeof (float)));

    glEnableVertexAttribArray (attribute->location);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}

sat_status_t sat_opengl_vbo_enable (sat_opengl_vbo_t *object)
{
    glBindBuffer (GL_ARRAY_BUFFER, object->id);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}

sat_status_t sat_opengl_vbo_disable (sat_opengl_vbo_t *object)
{
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    sat_status_t status = sat_status_set (&status, true, "");
    return status;
}