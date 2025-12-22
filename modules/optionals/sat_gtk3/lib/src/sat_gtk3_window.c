#include <sat_gtk3_window.h>
#include <gtk/gtk.h>

static void sat_gtk3_window_close_event (GtkWidget *widget, void *data);

sat_status_t sat_gtk3_window_open (sat_gtk3_window_t *object, sat_gtk3_window_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 window open error");

    object->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    if (object->window != NULL)
    {
        if (args->on_close != NULL)
        {
            object->on_close = args->on_close;
            object->data = args->data;
        }

        g_signal_connect (object->window, "destroy", G_CALLBACK (sat_gtk3_window_close_event), object);

        sat_status_set (&status, true, "");
    }


    return status;
}

void sat_gtk3_window_show (sat_gtk3_window_t *object)
{
    gtk_widget_show_all (object->window);
}

void sat_gtk3_window_add_widget (sat_gtk3_window_t *object, sat_gtk3_widget_t *widget)
{
    gtk_container_add (GTK_CONTAINER (object->window), widget->widget);
}

static void sat_gtk3_window_close_event (GtkWidget *widget, void *data)
{
    (void) widget;
    sat_gtk3_window_t *window = (sat_gtk3_window_t *) data;

    if (window->on_close != NULL)
    {
        window->on_close (window, window->data);
    }
}
