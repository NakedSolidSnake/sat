#include <sat.h>
#include <math.h>

#define DEGREE_TO_RAD 3.14159265359f / 180.0f
#define STEP_DEGREES 6.0f
#define STEP_RAD (STEP_DEGREES * DEGREE_TO_RAD)

typedef struct
{
    float x;
    float y;
} point_t;


sat_array_t *arc_generate (point_t center, float radius, float start_degrees, float end_degrees)
{
    sat_array_t *vertices;
    
    sat_array_create (&vertices, &(sat_array_args_t)
                                 {
                                   .object_size = sizeof(float),
                                   .size = 1,
                                   .mode = sat_array_mode_dynamic
                                 });

    // garante sentido anti-horário
    if (end_degrees < start_degrees)
        end_degrees += 360.0f;

    for (float deg = start_degrees; deg <= end_degrees; deg += STEP_DEGREES)
    {
        float rad = deg * DEGREE_TO_RAD;

        float x = center.x + radius * cosf (rad);
        float y = center.y + radius * sinf (rad);

        sat_array_add (vertices, &x);
        sat_array_add (vertices, &y);
    }

    return vertices;
}

int main (int argc, char** argv)
{
    char *vertex_shader = "shaders/vertex.vs";
    char *fragment_shader = "shaders/fragment.fs";

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

    status = sat_opengl_create_vao (opengl, "arc");
    assert (sat_status_get_result (&status) == true);

    sat_array_t *arc_vertices = arc_generate ((point_t){.x = 0.0f, .y = 0.0f}, 0.5f, 90.0f, 360.0f);

    sat_array_buffer_t arr_buffer;
    sat_array_get_buffer (arc_vertices, &arr_buffer);

    sat_opengl_attribute_t attributes [] = 
    {
      {.location = 0, .amount = 2, .elements = 2, .offset = 0},
    };

    status = sat_opengl_add_vbo_to_vao (opengl, "arc", &(sat_opengl_vbo_args_t)
                                                        {
                                                            .vertices = 
                                                            {
                                                                .list = arr_buffer.data,
                                                                .size = arr_buffer.size * sizeof (float)
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

        status = sat_opengl_enable_vao (opengl, "arc");
        assert (sat_status_get_result (&status) == true);

        status = sat_opengl_draw_object (opengl, sat_opengl_draw_type_line_strip, arr_buffer.size / 2);
        assert (sat_status_get_result (&status) == true);

        status = sat_opengl_draw (opengl);
    }

    status = sat_opengl_close (opengl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}