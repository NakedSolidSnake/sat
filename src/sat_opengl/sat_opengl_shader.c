#include <sat_opengl_shader.h>
#include <GL/glew.h>
#include <sat_file.h>
#include <stdlib.h>


static int sat_opengl_shader_get_type (sat_opengl_shader_type_t type);
static sat_status_t sat_opengl_shader_check_compile_status (sat_opengl_shader_t *object);

sat_status_t sat_opengl_shader_create_from_file (sat_opengl_shader_t *object, sat_opengl_shader_type_t type, const char *path)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl shader create from file error");

    char *buffer = NULL;

    do 
    {
        if (sat_file_exists (path) == false)
        {
            sat_status_set (&status, false, "sat opengl shader create from file error: file not found");
            break;
        }

        object->id = glCreateShader (sat_opengl_shader_get_type (type));
        if (object->id == 0)
        {
            break;
        }

        if (sat_file_read_to_buffer (path, (void **)&buffer, 4092) == false)
        {
            sat_status_set (&status, false, "sat opengl shader create from file error: cannot allocate shader");
            break;
        }

        glShaderSource (object->id, 1, (const char **)&buffer, NULL);

        glCompileShader (object->id);

        status = sat_opengl_shader_check_compile_status (object);

        free (buffer);

    } while (false);

    return status;
}

sat_status_t sat_opengl_shader_free (sat_opengl_shader_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl shader create from file error");

    if (glIsShader (object->id) == GL_TRUE)
    {
        glDeleteShader (object->id);

        sat_status_set (&status, true, "");
    }

    return status;
}

static int sat_opengl_shader_get_type (sat_opengl_shader_type_t type)
{
    return type == sat_opengl_shader_type_vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
}

static sat_status_t sat_opengl_shader_check_compile_status (sat_opengl_shader_t *object)
{
    sat_status_t status = sat_status_set (&status, true, "");
    int success;

    glGetShaderiv (object->id, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        char info [512] = {0};

        glGetShaderInfoLog (object->id, sizeof (info), NULL, info);

        // log this error

        sat_status_set (&status, false, "sat opengl shader compile error");
    }

    return status;    
}
