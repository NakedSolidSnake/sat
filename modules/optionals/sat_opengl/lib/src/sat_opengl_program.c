#include <sat_opengl_program.h>
#include <sat_iterator.h>
#include <GL/glew.h>

static sat_status_t sat_opengl_program_check_link_status (sat_opengl_shader_t *object);

sat_status_t sat_opengl_program_create (sat_opengl_program_t *object, sat_opengl_program_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl program create error");

    do 
    {
        object->id = glCreateProgram ();
        if (object->id == 0)
        {
            break;
        }

        status = sat_array_create (&object->shaders, &(sat_array_args_t)
                                                    {
                                                        .size = 1,
                                                        .object_size = sizeof (sat_opengl_shader_t),
                                                        .mode = sat_array_mode_dynamic,
                                                    });

        object->name = args->name;

    } while (false);

    return status;
}

sat_status_t sat_opengl_program_shader_add (sat_opengl_program_t *object, sat_opengl_shader_t *shader)
{
    return sat_array_add (object->shaders, (void *)shader);
}

sat_status_t sat_opengl_program_link (sat_opengl_program_t *object)
{
    sat_iterator_t iterator;
    sat_status_t status;
    uint32_t size;

    do 
    {
        status = sat_array_get_size (object->shaders, &size);
        if (size == 0)
        {
            break;
        }

        status = sat_iterator_open (&iterator, (sat_iterator_base_t *)object->shaders);

        sat_opengl_shader_t *shader = (sat_opengl_shader_t *) sat_iterator_next (&iterator);

        while (shader != NULL)
        {
            glAttachShader (object->id, shader->id);
            shader = (sat_opengl_shader_t *) sat_iterator_next (&iterator);
        }

        glLinkProgram (object->id);

        status = sat_opengl_program_check_link_status ((sat_opengl_shader_t*)object);
        sat_status_break_on_error (status);

        status = sat_opengl_program_shader_delete (object);

    } while (false);

    return status;
}

/* TODO make this function private */
sat_status_t sat_opengl_program_shader_delete (sat_opengl_program_t *object)
{
    sat_iterator_t iterator;
    sat_status_t status;
    uint32_t size;

    do 
    {
        status = sat_array_get_size (object->shaders, &size);
        if (size == 0)
        {
            break;
        }

        status = sat_iterator_open (&iterator, (sat_iterator_base_t *)object->shaders);

        sat_opengl_shader_t *shader = (sat_opengl_shader_t *) sat_iterator_next (&iterator);

        while (shader != NULL)
        {
            sat_opengl_shader_free (shader);

            shader = (sat_opengl_shader_t *) sat_iterator_next (&iterator);
        }

    } while (false);

    return status;
}

sat_status_t sat_opengl_program_delete (sat_opengl_program_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl program delete error");

    if (glIsProgram (object->id) == GL_TRUE)
    {
        glDeleteProgram (object->id);

        status = sat_array_destroy (object->shaders);
    }

    return status;
}

sat_status_t sat_opengl_program_enable (sat_opengl_program_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl program enable error");

    if (glIsProgram (object->id) == GL_TRUE)
    {
        glUseProgram (object->id);

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_opengl_program_check_link_status (sat_opengl_shader_t *object)
{
    sat_status_t status = sat_status_set (&status, true, "");
    int success;

    glGetShaderiv (object->id, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        char info [512] = {0};

        glGetProgramInfoLog (object->id, sizeof (info), NULL, info);

        // log this error

        sat_status_set (&status, false, "sat opengl program link error");
    }

    return status;    
}

sat_status_t sat_opengl_program_set_bool (sat_opengl_program_t *object, const char *const name, sat_opengl_value_send_t send, const sat_opengl_value_bool_t *const value)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl program set bool error");

    GLint location = glGetUniformLocation (object->id, name);

    // TODO Test location first.

    switch (send)
    {
        case sat_opengl_value_send_one:
        glUniform1i (location, (int) value->first);
        sat_status_set (&status, true, "");
        break;

        case sat_opengl_value_send_two:
        glUniform2i (location, (int) value->first, (int) value->second);
        sat_status_set (&status, true, "");
        break;

        case sat_opengl_value_send_three:
        glUniform3i (location, (int) value->first, (int) value->second, (int) value->third);
        sat_status_set (&status, true, "");
        break;

        case sat_opengl_value_send_four:
        glUniform4i (location, (int) value->first, (int) value->second, (int) value->third, (int) value->fourth);
        sat_status_set (&status, true, "");
        break;
    }

    return status;
}

sat_status_t sat_opengl_program_set_int (sat_opengl_program_t *object, const char *const name, sat_opengl_value_send_t send, const sat_opengl_value_int_t *const value)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl program set int error");

    GLint location = glGetUniformLocation (object->id, name);

    // TODO Test location first.

    switch (send)
    {
        case sat_opengl_value_send_one:
        glUniform1i (location, value->first);
        sat_status_set (&status, true, "");
        break;

        case sat_opengl_value_send_two:
        glUniform2i (location, value->first, value->second);
        sat_status_set (&status, true, "");
        break;

        case sat_opengl_value_send_three:
        glUniform3i (location, value->first, value->second, value->third);
        sat_status_set (&status, true, "");
        break;

        case sat_opengl_value_send_four:
        glUniform4i (location, value->first, value->second, value->third, value->fourth);
        sat_status_set (&status, true, "");
        break;
    }

    return status;
}

sat_status_t sat_opengl_program_set_float (sat_opengl_program_t *object, const char *const name, sat_opengl_value_send_t send, const sat_opengl_value_float_t *const value)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl program set float error");

    GLint location = glGetUniformLocation (object->id, name);

    // TODO Test location first.

    switch (send)
    {
        case sat_opengl_value_send_one:
        glUniform1f (location, value->first);
        sat_status_set (&status, true, "");
        break;

        case sat_opengl_value_send_two:
        glUniform2f (location, value->first, value->second);
        sat_status_set (&status, true, "");
        break;

        case sat_opengl_value_send_three:
        glUniform3f (location, value->first, value->second, value->third);
        sat_status_set (&status, true, "");
        break;

        case sat_opengl_value_send_four:
        glUniform4f (location, value->first, value->second, value->third, value->fourth);
        sat_status_set (&status, true, "");
        break;
    }

    return status;
}

sat_status_t sat_opengl_program_set_matrix (sat_opengl_program_t *object, const char *const name, const sat_opengl_matrix_t *const matrix)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl program set matrix error");

    GLint location = glGetUniformLocation (object->id, name);

    // TODO Test location first.

    switch (matrix->type)
    {
        case sat_opengl_matrix_type_2x2:
            glUniformMatrix2fv (location, 1, GL_FALSE, (const GLfloat *)matrix->matrix_2x2);
            sat_status_set (&status, true, "");
        break;

        case sat_opengl_matrix_type_3x3:
            glUniformMatrix3fv (location, 1, GL_FALSE, (const GLfloat *)matrix->matrix_3x3);
            sat_status_set (&status, true, "");
        break;

        case sat_opengl_matrix_type_4x4:
            glUniformMatrix4fv (location, 1, GL_FALSE, (const GLfloat *)matrix->matrix_4x4);
            sat_status_set (&status, true, "");
        break;
    }

    return status;
}