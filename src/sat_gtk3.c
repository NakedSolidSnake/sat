#include <sat_gtk3.h>
#include <string.h>
#include <gtk/gtk.h>

sat_status_t sat_gtk3_init (sat_gtk3_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_gtk3_t));

        object->initialized = true;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_gtk3_open (sat_gtk3_t *object, sat_gtk3_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 open error");

    if (object != NULL && object->initialized == true && args != NULL && args->argc >= 1)
    {
        gtk_init (&args->argc, &args->argv);

        status = sat_gtk3_window_open (&object->window);
    }

    return status;
}

sat_status_t sat_gtk3_run (sat_gtk3_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 run error");

    if (object != NULL && object->initialized == true)
    {
        sat_gtk3_window_show (&object->window);

        gtk_main ();

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_gtk3_close (sat_gtk3_t *object)
{
     sat_status_t status = sat_status_set (&status, false, "sat gtk3 close error");

    if (object != NULL && object->initialized == true)
    {

        sat_status_set (&status, true, "");
    }

    return status;
}