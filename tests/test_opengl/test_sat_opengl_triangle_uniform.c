#include <sat.h>
#include <assert.h>
#include <math.h>

int main (int argc, char *argv [])
{

    sat_opengl_t *opengl;

    float vertices [] = 
    {
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    assert (argc == 3);

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

    char *vertex = argv [1];
    char *fragment = argv [2];

    status = sat_opengl_create_program (opengl, "triangle");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_add_shader_to_program (opengl, "triangle", sat_opengl_shader_type_vertex, vertex);
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_add_shader_to_program (opengl, "triangle", sat_opengl_shader_type_fragment, fragment);
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_compile_program (opengl, "triangle");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_create_vao (opengl, "triangle");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_add_vbo_to_vao (opengl, "triangle", &(sat_opengl_vbo_args_t)
                                                            {
                                                              .name = "vbo",
                                                              .vertices = 
                                                              {
                                                                .list = vertices,
                                                                .size = sizeof (vertices)
                                                              },
                                                              .attribute = 
                                                              {
                                                                .location = 0,
                                                                .amount = 3,
                                                                .elements = 3,
                                                                .offset = 0
                                                              }
                                                            });
    assert (sat_status_get_result (&status) == true);

    while (sat_status_get_result (&status) == true)
    {

      float time = 0.0f;

      sat_opengl_set_color (opengl, (sat_opengl_color_t){.red = 0.2f, .green = 0.3f, .blue = 0.3f, .alpha = 1.0f});

      status = sat_opengl_enable_program (opengl, "triangle");
      assert (sat_status_get_result (&status) == true);

      status = sat_opengl_enable_vao (opengl, "triangle");
      assert (sat_status_get_result (&status) == true);

      status = sat_opengl_get_time (opengl, &time);
      assert (sat_status_get_result (&status) == true);

      float color = sin (time) / 2.0f;

      status = sat_opengl_send_shader_value (opengl, "triangle", "input_color", &(sat_opengl_value_t)
                                                                                {
                                                                                  .type = sat_opengl_value_type_float,
                                                                                  .send = sat_opengl_value_send_one,
                                                                                  .floats = 
                                                                                  {
                                                                                    .first = color
                                                                                  }
                                                                                });

      status = sat_opengl_draw (opengl, sat_opengl_draw_type_triangles, 3);
    }

    status = sat_opengl_close (opengl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}