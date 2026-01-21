#include <sat.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static sat_array_t *vertices_create (uint16_t lines, uint16_t step)
{
    sat_array_t *array;
    sat_array_create (&array, &(sat_array_args_t)
                                {
                                    .size = lines,
                                    .object_size = sizeof (float),
                                    .mode = sat_array_mode_dynamic,
                                    .notification =
                                    {
                                        .on_increase = NULL,
                                        .user = NULL
                                    }
                                });

    float steps = (float)step / (float)lines;

    printf("Steps: %f\n", steps);

    // Vertical lines
    for (uint16_t index = 0; index < lines; index++)
    {
        float x = -1.0f + (steps * index);

        sat_array_add (array, &x);                     // x1
        sat_array_add (array, &(float) {-1.0f} );     // y1
        sat_array_add (array, &x);                     // x2
        sat_array_add (array, &(float) {1.0f} );      // y2
    }

    // exit (0);

    // Horizontal lines
    for (uint16_t index = 0; index < lines; index++)
    {
        float y = -1.0f + (steps * index);

        sat_array_add (array, &(float) {-1.0f} );     // x1
        sat_array_add (array, &y);                     // y1
        sat_array_add (array, &(float) {1.0f} );      // x2
        sat_array_add (array, &y);                     // y2
    }

    return array;
}

int main (int argc, char** argv)
{
    sat_opengl_t *opengl;

    sat_array_t *vertices = vertices_create (10, 2);

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

    status = sat_opengl_add_shader_to_program (opengl, "application", sat_opengl_shader_type_vertex, "./shaders/vertex");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_add_shader_to_program (opengl, "application", sat_opengl_shader_type_fragment, "./shaders/fragment");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_compile_program (opengl, "application");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_create_vao (opengl, "grid");
    assert (sat_status_get_result (&status) == true);

    sat_array_buffer_t buffer;
    sat_array_get_buffer (vertices, &buffer);

    printf("Total floats: %u, vertices: %u\n", buffer.size, buffer.size / 2);

    sat_opengl_attribute_t attributes [] = 
    {
        {.location = 0, .amount = 2, .elements = 2, .offset = 0}
    };

    status = sat_opengl_add_vbo_to_vao (opengl, "grid", &(sat_opengl_vbo_args_t)
                                                            {
                                                              .name = "vbo",
                                                              .vertices = 
                                                              {
                                                                .list = buffer.data,
                                                                .size = buffer.size * sizeof (float)
                                                              },
                                                              .attributes = 
                                                              {
                                                                .list = attributes,
                                                                .amount = 1
                                                              }
                                                            });
    assert (sat_status_get_result (&status) == true);

    while (true)
    {
        sat_opengl_set_color (opengl, (sat_opengl_color_t){.red = 0.2f, .green = 0.3f, .blue = 0.3f, .alpha = 1.0f});

        status = sat_opengl_enable_program (opengl, "application");
        assert (sat_status_get_result (&status) == true);

        status = sat_opengl_enable_vao (opengl, "grid");
        assert (sat_status_get_result (&status) == true);

        printf("Desenhando %u vertices\n", buffer.size / 2);

        status = sat_opengl_draw_object (opengl, sat_opengl_draw_type_lines, buffer.size / 2);
        assert (sat_status_get_result (&status) == true);

        status = sat_opengl_draw (opengl);
    }

    status = sat_opengl_close (opengl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}