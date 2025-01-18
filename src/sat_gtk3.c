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

        sat_gtk3_window_args_t window_args = 
        {
            .on_close = args->on_close,
            .data = args->data,
        };

        status = sat_gtk3_window_open (&object->window, &window_args);
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
        gtk_main_quit ();

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_gtk3_add_widget (sat_gtk3_t *object, sat_gtk3_widget_t *widget)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 close error");

    if (object != NULL && object->initialized == true && widget != NULL && widget->widget != NULL)
    {
        sat_gtk3_window_add_widget (&object->window, widget);

        sat_status_set (&status, true, "");
    }

    return status;
}