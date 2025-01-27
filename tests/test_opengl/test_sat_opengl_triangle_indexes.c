#include <sat.h>
#include <assert.h>

int main (int argc, char *argv [])
{

    sat_opengl_t *opengl;

    float vertices [] = 
    {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f,
    };

    unsigned int indexes [] = 
    {
      0, 1, 3,
      1, 2, 3
    };

    sat_opengl_attribute_t attributes [] = 
    {
      {.location = 0, .amount = 3, .elements = 3, .offset = 0},   
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
                                                              .attributes = 
                                                              {
                                                                .list = attributes,
                                                                .amount = 1
                                                              },
                                                              .indexes = 
                                                              {
                                                                .list = indexes,
                                                                .size = sizeof (indexes)
                                                              }
                                                            });
    assert (sat_status_get_result (&status) == true);

    while (sat_status_get_result (&status) == true)
    {
      sat_opengl_set_color (opengl, (sat_opengl_color_t){.red = 0.2f, .green = 0.3f, .blue = 0.3f, .alpha = 1.0f});

      status = sat_opengl_enable_program (opengl, "triangle");
      assert (sat_status_get_result (&status) == true);

      status = sat_opengl_enable_vao (opengl, "triangle");
      assert (sat_status_get_result (&status) == true);

      status = sat_opengl_draw (opengl, sat_opengl_draw_type_elements, 6);
    }

    status = sat_opengl_close (opengl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}