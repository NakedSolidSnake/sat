#include <sat.h>
#include <assert.h>

static void on_close (void *object, void *data);

int main (int argc, char *argv [])
{
    sat_gtk3_t gtk;
    sat_gtk3_args_t args =
    {
        .argc = argc,
        .argv = argv,
        .on_close = on_close,
        .data = &gtk
    };

    sat_status_t status = sat_gtk3_init (&gtk);
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_open (&gtk, &args);
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_run (&gtk);
    assert (sat_status_get_result (&status) == true);

    return 0;
}

static void on_close (void *object, void *data)
{
    sat_gtk3_t *gtk = (sat_gtk3_t *)data;

    sat_gtk3_close (gtk);
}