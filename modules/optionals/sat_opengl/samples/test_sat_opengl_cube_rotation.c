#include <sat.h>
#include <assert.h>
#include <math.h>

int main (int argc, char *argv [])
{

    sat_opengl_t *opengl;

    float vertices [] =
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // unsigned int indexes [] = 
    // {
    //     0, 1, 3,
    //     1, 2, 3
    // };

    sat_opengl_attribute_t attributes [] = 
    {
      {.location = 0, .amount = 3, .elements = 5, .offset = 0},
      {.location = 1, .amount = 2, .elements = 5, .offset = 3},
    };

    assert (argc == 5);

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
    char *texture = argv [3];
    char *texture2 = argv [4];

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
                                                              // .indexes = 
                                                              // {
                                                              //   .list = indexes,
                                                              //   .size = sizeof (indexes)
                                                              // },
                                                              .attributes = 
                                                              {
                                                                .list = attributes,
                                                                .amount = 2
                                                              }
                                                            });
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_texture_container_create (opengl, "textures");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_texture_container_add (opengl, "textures", &(sat_opengl_texture_args_t)
                                                                   {
                                                                    .type = sat_opengl_texture_type_2D,
                                                                    .format = sat_opengl_texture_format_jpg,
                                                                    .filename = texture,
                                                                    .flip = true,
                                                                   });
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_texture_container_add (opengl, "textures", &(sat_opengl_texture_args_t)
                                                                   {
                                                                    .type = sat_opengl_texture_type_2D,
                                                                    .format = sat_opengl_texture_format_png,
                                                                    .filename = texture2,
                                                                    .flip = true,
                                                                   });
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_enable_program (opengl, "triangle");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_send_shader_value (opengl, "triangle", "texture1", &(sat_opengl_value_t)
                                                                          {
                                                                            .type = sat_opengl_value_type_int,
                                                                            .send = sat_opengl_value_send_one,
                                                                            .ints = 
                                                                            {
                                                                              .first = 0
                                                                            }
                                                                          });

    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_send_shader_value (opengl, "triangle", "texture2", &(sat_opengl_value_t)
                                                                          {
                                                                            .type = sat_opengl_value_type_int,
                                                                            .send = sat_opengl_value_send_one,
                                                                            .ints = 
                                                                            {
                                                                              .first = 1
                                                                            }
                                                                          });

    assert (sat_status_get_result (&status) == true);

    while (sat_status_get_result (&status) == true)
    {

      sat_opengl_set_color (opengl, (sat_opengl_color_t){.red = 0.2f, .green = 0.3f, .blue = 0.3f, .alpha = 1.0f});

      status = sat_opengl_texture_container_enable (opengl, "textures");
      assert (sat_status_get_result (&status) == true);

      mat4 model = GLM_MAT4_IDENTITY_INIT;
      mat4 view = GLM_MAT4_IDENTITY_INIT;
      mat4 projection = GLM_MAT4_IDENTITY_INIT;

      vec3 model_rotate = {0.5f, 1.0f, 0.0f};
      vec3 view_translate = {0.0f, 0.0f, -3.0f};

      float time;

      status = sat_opengl_get_time (opengl, &time);
      assert (sat_status_get_result (&status) == true);

      glm_rotate (model, glm_rad (50.0f) * time, model_rotate);
      glm_translate (view, view_translate);

      sat_opengl_dimension_t dimension;

      status = sat_opengl_get_window_dimension (opengl, &dimension);
      assert (sat_status_get_result (&status) == true);

      glm_perspective (glm_rad (45.0f), dimension.width / dimension.height, 0.1f, 100.0f, projection);

      status = sat_opengl_enable_program (opengl, "triangle");
      assert (sat_status_get_result (&status) == true);

      status = sat_opengl_send_shader_matrix (opengl, "triangle", "model", &(sat_opengl_matrix_t){.type = sat_opengl_matrix_type_4x4, .matrix_4x4 = &model});
      assert (sat_status_get_result (&status) == true);

      status = sat_opengl_send_shader_matrix (opengl, "triangle", "view", &(sat_opengl_matrix_t){.type = sat_opengl_matrix_type_4x4, .matrix_4x4 = &view});
      assert (sat_status_get_result (&status) == true);

      status = sat_opengl_send_shader_matrix (opengl, "triangle", "projection", &(sat_opengl_matrix_t){.type = sat_opengl_matrix_type_4x4, .matrix_4x4 = &projection});
      assert (sat_status_get_result (&status) == true);

      status = sat_opengl_enable_vao (opengl, "triangle");
      assert (sat_status_get_result (&status) == true);

      status = sat_opengl_draw_object (opengl, sat_opengl_draw_type_triangles, 36);
      assert (sat_status_get_result (&status) == true);

      status = sat_opengl_draw (opengl);
    }

    status = sat_opengl_close (opengl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}