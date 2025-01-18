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

    status = sat_opengl_run (opengl);
    assert (sat_status_get_result (&status) == true);

    status = sat_opengl_close (opengl);
    assert (sat_status_get_result (&status) == true);

    return 0;
}