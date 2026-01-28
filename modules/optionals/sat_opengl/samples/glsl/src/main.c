#include <sat.h>

int main (int argc, char** argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <vertex_shader_path> <fragment_shader_path>\n", argv[0]);
        return -1;
    }

    char *vertex_shader = argv[1];
    char *fragment_shader = argv[2];

    sat_opengl_t *opengl;

    sat_status_t status = sat_opengl_create (&opengl, &(sat_opengl_args_t)
                                                      {
                                                        .window = 
                                                        {
                                                            .height = 600,
                                                            .width = 800,
                                                            .title = "Test Window"
                                                        }
                                                      });
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_create_program (opengl, "application");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_add_shader_to_program (opengl, "application", sat_opengl_shader_type_vertex, vertex_shader);
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_add_shader_to_program (opengl, "application", sat_opengl_shader_type_fragment, fragment_shader);
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_compile_program (opengl, "application");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_create_vao (opengl, "point");
    assert (sat_status_get_result (&status) == true);

    while (true)
    {
        sat_opengl_set_color (opengl, (sat_opengl_color_t){.red = 0.2f, .green = 0.3f, .blue = 0.3f, .alpha = 1.0f});

        status = sat_opengl_enable_program (opengl, "application");
        assert (sat_status_get_result (&status) == true);

        status = sat_opengl_enable_vao (opengl, "point");
        assert (sat_status_get_result (&status) == true);

        status = sat_opengl_draw_object (opengl, sat_opengl_draw_type_points, 1);
        assert (sat_status_get_result (&status) == true);

        status = sat_opengl_draw (opengl);
    }

    status = sat_opengl_close (opengl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}