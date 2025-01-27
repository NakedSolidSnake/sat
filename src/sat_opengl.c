#include <sat_opengl.h>
#include <sat_opengl_window.h>
#include <sat_opengl_program.h>
#include <sat_opengl_vao.h>
#include <sat_opengl_vbo.h>
#include <sat_opengl_ebo.h>
#include <sat_opengl_container_texture.h>

#include <sat_set.h>
#include <sat_iterator.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string.h>

struct sat_opengl_t 
{
    sat_opengl_window_t window;
    bool initialized;
    sat_set_t *programs;
    sat_set_t *vaos;
    sat_set_t *textures;
};

static sat_status_t sat_opengl_check_args (sat_opengl_args_t *args);
static sat_status_t sat_opengl_init (void);
static bool sat_opengl_is_program_equal (void *element, void *new_element);
static bool sat_opengl_is_vao_equal (void *element, void *new_element);
static bool sat_opengl_is_textures_equal (void *element, void *new_element);
static void sat_opengl_draw_by (sat_opengl_draw_type_t type, uint32_t vertices_amount);

sat_status_t sat_opengl_create (sat_opengl_t **object, sat_opengl_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl create error");

    do 
    {
        if (object == NULL)
        {
            break;
        }

        status = sat_opengl_check_args (args);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_opengl_init ();
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        sat_opengl_t *opengl = calloc (1, sizeof (struct sat_opengl_t));
        if (opengl == NULL)
        {
            break;
        }

        status = sat_opengl_window_create (&opengl->window, &(sat_opengl_window_args_t)
                                                            {
                                                                .height = args->window.height,
                                                                .width = args->window.width,
                                                                .title = args->window.title
                                                            });
        if (sat_status_get_result (&status) == false)
        {
            free (opengl);
            break;
        }

        status = sat_set_create (&opengl->programs, &(sat_set_args_t)
                                                    {
                                                        .size = 3,
                                                        .object_size = sizeof (sat_opengl_program_t),
                                                        .is_equal = sat_opengl_is_program_equal,
                                                        .mode = sat_set_mode_dynamic
                                                    });
        if (sat_status_get_result (&status) == false)
        {
            sat_opengl_window_close (&opengl->window);

            free (opengl);
            break;
        }

        status = sat_set_create (&opengl->vaos, &(sat_set_args_t)
                                                    {
                                                        .size = 3,
                                                        .object_size = sizeof (sat_opengl_vao_t),
                                                        .is_equal = sat_opengl_is_vao_equal,
                                                        .mode = sat_set_mode_dynamic
                                                    });
        if (sat_status_get_result (&status) == false)
        {
            sat_opengl_window_close (&opengl->window);

            
            free (opengl);
            break;
        }

        status = sat_set_create (&opengl->textures, &(sat_set_args_t)
                                                    {
                                                        .size = 3,
                                                        .object_size = sizeof (sat_opengl_container_texture_t),
                                                        .is_equal = sat_opengl_is_textures_equal,
                                                        .mode = sat_set_mode_dynamic
                                                    });
        if (sat_status_get_result (&status) == false)
        {
            sat_opengl_window_close (&opengl->window);

            
            free (opengl);
            break;
        }

        if (glewInit () == GL_TRUE)
        {
            sat_opengl_window_close (&opengl->window);

            free (opengl);
            break;
        }

        opengl->initialized = true;
        *object = opengl;

    } while (false);

    return status;
}

sat_status_t sat_opengl_run (sat_opengl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl run error");

    if (object != NULL && object->initialized == true)
    {
        sat_opengl_window_run (&object->window);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_opengl_create_program (sat_opengl_t *object, const char *name)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl create program error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0)
    {
        sat_opengl_program_t program;

        do 
        {
            status = sat_opengl_program_create (&program, &(sat_opengl_program_args_t){.name = name});
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            status = sat_set_add (object->programs, &program);
            if (sat_status_get_result (&status) == false)
            {
                sat_opengl_program_delete (&program);
                break;
            }

            sat_status_set (&status, true, "");

        } while (false);
    }

    return status;
}

sat_status_t sat_opengl_add_shader_to_program (sat_opengl_t *object, const char *name, sat_opengl_shader_type_t type, const char *filename)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl create program error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0 && filename != NULL)
    {
        sat_iterator_t iterator;

        do 
        {
            status = sat_iterator_open (&iterator, object->programs);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_opengl_program_t *program = (sat_opengl_program_t *) sat_iterator_next (&iterator);

            while (program != NULL)
            {
                if (strcmp (program->name, name) == 0)
                {
                    sat_opengl_shader_t shader;
                    status = sat_opengl_shader_create_from_file (&shader, type, filename);
                    if (sat_status_get_result (&status) == false)
                    {
                        break;
                    }

                    status = sat_opengl_program_shader_add (program, &shader);

                    break;
                }

                program = (sat_opengl_program_t *) sat_iterator_next (&iterator);
            }

        } while (false);
    }

    return status;
}

sat_status_t sat_opengl_compile_program (sat_opengl_t *object, const char *name)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl create program error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0)
    {
        sat_iterator_t iterator;

        do 
        {
            status = sat_iterator_open (&iterator, object->programs);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_opengl_program_t *program = (sat_opengl_program_t *) sat_iterator_next (&iterator);

            while (program != NULL)
            {
                if (strcmp (program->name, name) == 0)
                {
                    status = sat_opengl_program_link (program);

                    break;
                }

                program = (sat_opengl_program_t *) sat_iterator_next (&iterator);
            }

        } while (false);
    }

    return status;
}

sat_status_t sat_opengl_close (sat_opengl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl close error");

    if (object != NULL && object->initialized == true)
    {
        sat_opengl_window_close (&object->window);
        
        sat_set_destroy (object->programs);

        sat_set_destroy (object->vaos);

        glfwTerminate ();

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_opengl_create_vao (sat_opengl_t *object, const char *name)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl create vao error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0)
    {
        sat_opengl_vao_t vao;

        do 
        {
            status = sat_opengl_vao_create (&vao, name);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            status = sat_set_add (object->vaos, &vao);
            if (sat_status_get_result (&status) == false)
            {
                sat_opengl_vao_destroy (&vao);
                break;
            }

            sat_status_set (&status, true, "");

        } while (false);
    }

    return status;
}

sat_status_t sat_opengl_enable_vao (sat_opengl_t *object, const char *name)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl enable vao error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0)
    {
        sat_iterator_t iterator;

        do 
        {
            status = sat_iterator_open (&iterator, object->vaos);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_opengl_vao_t *vao = (sat_opengl_vao_t *) sat_iterator_next (&iterator);

            while (vao != NULL)
            {
                if (strcmp (vao->name, name) == 0)
                {
                    sat_opengl_vao_enable (vao);

                    break;
                }

                vao = (sat_opengl_vao_t *) sat_iterator_next (&iterator);
            }

        } while (false);
    }

    return status;
}

sat_status_t sat_opengl_enable_program (sat_opengl_t *object, const char *name)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl create program error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0)
    {
        sat_iterator_t iterator;

        do 
        {
            status = sat_iterator_open (&iterator, object->programs);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_opengl_program_t *program = (sat_opengl_program_t *) sat_iterator_next (&iterator);

            while (program != NULL)
            {
                if (strcmp (program->name, name) == 0)
                {
                    sat_opengl_program_enable (program);

                    break;
                }

                program = (sat_opengl_program_t *) sat_iterator_next (&iterator);
            }

        } while (false);
    }

    return status;
}

sat_status_t sat_opengl_draw (sat_opengl_t *object, sat_opengl_draw_type_t type, uint32_t vertices_amount)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl draw error");

    if (object != NULL && object->initialized == true)
    {
        // glDrawArrays (GL_TRIANGLES, 0, 3);

        sat_opengl_draw_by (type, vertices_amount);

        if (sat_opengl_window_run (&object->window) == true)
        {
            sat_status_set (&status, true, "");
        }

    }

    return status;
}

sat_status_t sat_opengl_set_color (sat_opengl_t *object, sat_opengl_color_t color)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl set color error");

    if (object != NULL && object->initialized == true)
    {
        glClearColor (color.red, color.green, color.blue, color.alpha);
        glClear (GL_COLOR_BUFFER_BIT);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_opengl_add_vbo_to_vao (sat_opengl_t *object, const char *name, sat_opengl_vbo_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl add vbo to vao error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0 && args != NULL)
    {
        sat_iterator_t iterator;

        do 
        {
            status = sat_iterator_open (&iterator, object->programs);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_opengl_vao_t *vao = (sat_opengl_vao_t *) sat_iterator_next (&iterator);

            while (vao != NULL)
            {
                if (strcmp (vao->name, name) == 0)
                {
                    sat_opengl_vao_enable (vao);

                    sat_opengl_vbo_t vbo;

                    sat_opengl_vbo_create (&vbo, args->name);
                    sat_opengl_vbo_enable (&vbo);

                    sat_opengl_vbo_set_vertices (&vbo, &args->vertices);

                    sat_opengl_vbo_set_attributes (&vbo, &args->attributes);

                    if (args->indexes.list != NULL && args->indexes.size > 0)
                    {
                        sat_opengl_ebo_t ebo;

                        sat_opengl_ebo_create (&ebo);
                        sat_opengl_ebo_enable (&ebo);

                        sat_opengl_ebo_set_indexes (&ebo, &args->indexes);
                    }

                    sat_opengl_vbo_disable (&vbo);
                    sat_opengl_vao_disable (vao);

                    break;
                }

                vao = (sat_opengl_vao_t *) sat_iterator_next (&iterator);
            }

        } while (false);
    }

    return status;
}


sat_status_t sat_opengl_texture_container_create (sat_opengl_t *object, const char *name)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl texture container create error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0)
    {
        sat_opengl_container_texture_t container;

        sat_opengl_container_texture_init (&container, name);

        status = sat_set_add (object->textures, &container);
    }

    return status;
}

sat_status_t sat_opengl_texture_container_add (sat_opengl_t *object, const char *name, sat_opengl_texture_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl texture container add error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0 && args != NULL)
    {
        sat_iterator_t iterator;

        do 
        {
            status = sat_iterator_open (&iterator, object->textures);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_opengl_container_texture_t *container = (sat_opengl_container_texture_t *) sat_iterator_next (&iterator);

            while (container != NULL)
            {
                if (strcmp (container->name, name) == 0)
                {
                    sat_opengl_texture_t texture;

                    status = sat_opengl_texture_open (&texture, args);

                    if (sat_status_get_result (&status) == true)
                    {
                        status = sat_opengl_container_texture_add (container, &texture);
                    }

                    break;
                }

                container = (sat_opengl_container_texture_t *) sat_iterator_next (&iterator);
            }

        } while (false);
    }

    return status;
}

sat_status_t sat_opengl_texture_container_enable (sat_opengl_t *object, const char *name)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl texture container enable error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0)
    {
        sat_iterator_t iterator;

        do 
        {
            status = sat_iterator_open (&iterator, object->textures);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_opengl_container_texture_t *container = (sat_opengl_container_texture_t *) sat_iterator_next (&iterator);

            while (container != NULL)
            {
                if (strcmp (container->name, name) == 0)
                {
                   
                    sat_opengl_container_texture_enable (container);

                    break;
                }

                container = (sat_opengl_container_texture_t *) sat_iterator_next (&iterator);
            }

        } while (false);
    }

    return status;
}

sat_status_t sat_opengl_send_shader_value (sat_opengl_t *object, const char *name, const char *param, const sat_opengl_value_t *value)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl send shader value error");

    if (object != NULL && object->initialized == true && name != NULL && strlen (name) > 0)
    {
        sat_iterator_t iterator;

        do 
        {
            status = sat_iterator_open (&iterator, object->programs);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_opengl_program_t *program = (sat_opengl_program_t *) sat_iterator_next (&iterator);

            while (program != NULL)
            {
                if (strcmp (program->name, name) == 0)
                {
                    switch (value->type)
                    {
                        case sat_opengl_value_type_bool:
                        status = sat_opengl_program_set_bool (program, param, value->send, &value->bools);
                        break;

                        case sat_opengl_value_type_int:
                        status = sat_opengl_program_set_int (program, param, value->send, &value->ints);
                        break;

                        case sat_opengl_value_type_float:
                        status = sat_opengl_program_set_float (program, param, value->send, &value->floats);
                        break;
                    }
                    break;
                }

                program = (sat_opengl_program_t *) sat_iterator_next (&iterator);
            }

        } while (false);
    }

    return status;
}

sat_status_t sat_opengl_get_time (sat_opengl_t *object, float *value)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl get time error");

    if (object != NULL && object->initialized == true && value != NULL)
    {
        *value = glfwGetTime ();

        sat_status_set (&status, true, "");
    }

    return status;
}


static sat_status_t sat_opengl_check_args (sat_opengl_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl check args error");

    if (args->window.height > 0 &&
        args->window.width > 0 &&
        args->window.title != NULL && 
        strlen (args->window.title) > 0)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_opengl_init (void)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl init error");

    if (glfwInit () == GL_TRUE)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static bool sat_opengl_is_program_equal (void *element, void *new_element)
{
    bool status = false;

    sat_opengl_program_t *program = (sat_opengl_program_t *)element;
    sat_opengl_program_t *new_program= (sat_opengl_program_t *)new_element;

    if (strcmp (program->name, new_program->name) == 0)
    {
        status = true;
    }

    return status;
}

static bool sat_opengl_is_vao_equal (void *element, void *new_element)
{
    bool status = false;

    sat_opengl_vao_t *vao = (sat_opengl_vao_t *)element;
    sat_opengl_vao_t *new_vao= (sat_opengl_vao_t *)new_element;

    if (strcmp (vao->name, new_vao->name) == 0)
    {
        status = true;
    }

    return status;
}

static bool sat_opengl_is_textures_equal (void *element, void *new_element)
{
    bool status = false;

    sat_opengl_container_texture_t *container = (sat_opengl_container_texture_t *)element;
    sat_opengl_container_texture_t *new_container = (sat_opengl_container_texture_t *)new_element;

    if (strcmp (container->name, new_container->name) == 0)
    {
        status = true;
    }

    return status;
}

static void sat_opengl_draw_by (sat_opengl_draw_type_t type, uint32_t vertices_amount)
{
    if (type == sat_opengl_draw_type_triangles)
    {
        glDrawArrays (GL_TRIANGLES, 0, vertices_amount);
    }
    else 
    {
        glDrawElements (GL_TRIANGLES, vertices_amount, GL_UNSIGNED_INT, 0);
    }
}