#include <sat.h>
#include <assert.h>

int main (int argc, char *argv [])
{

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

    char *vertex = "./shaders/vertex.shader";
    char *fragment = "./shaders/fragment.shader";

    status = sat_opengl_create_program (opengl, "test");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_add_shader_to_program (opengl, "test", sat_opengl_shader_type_vertex, vertex);
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_add_shader_to_program (opengl, "test", sat_opengl_shader_type_fragment, fragment);
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_compile_program (opengl, "test");
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_close (opengl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}