#include <sat.h>
#include <assert.h>

int main (int argc, char *argv [])
{
    sat_gtk3_t gtk;
    sat_gtk3_args_t args =
    {
        .argc = argc,
        .argv = argv
    };

    sat_status_t status = sat_gtk3_init (&gtk);
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_open (&gtk, &args);
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_run (&gtk);
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_close (&gtk);

    return 0;
}
