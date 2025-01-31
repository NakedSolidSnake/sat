#include <sat_opengl_window.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void sat_opengl_window_set_version (void);
static void sat_opengl_window_copy_to_context (sat_opengl_window_t *object, sat_opengl_window_args_t *args);

sat_status_t sat_opengl_window_create (sat_opengl_window_t *object, sat_opengl_window_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl window create error");

    sat_opengl_window_set_version ();

    sat_opengl_window_copy_to_context (object, args);

    object->handle = (GLFWwindow *) glfwCreateWindow (object->width,
                                                      object->height,
                                                      object->title,
                                                      NULL,
                                                      NULL);

    if (object->handle != NULL)
    {
        glfwMakeContextCurrent (object->handle);

        glEnable (GL_DEPTH_TEST);

        sat_status_set (&status, true, "");
    }

    return status;
}

bool sat_opengl_window_run (sat_opengl_window_t *object)
{
    bool status = true;

    if (glfwWindowShouldClose (object->handle))
    {
        status = false;
    }

    glfwSwapBuffers (object->handle);
    glfwPollEvents ();

    return status;
}

void sat_opengl_window_close (sat_opengl_window_t *object)
{
    glfwDestroyWindow (object->handle);
}

static void sat_opengl_window_set_version (void)
{
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static void sat_opengl_window_copy_to_context (sat_opengl_window_t *object, sat_opengl_window_args_t *args)
{
    object->height = args->height;
    object->width = args->width;
    object->title = args->title;
}