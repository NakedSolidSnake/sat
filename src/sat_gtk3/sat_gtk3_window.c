#include <sat_gtk3_window.h>
#include <gtk/gtk.h>

static void sat_gtk3_window_close_event (GtkWidget *window, void *data);

sat_status_t sat_gtk3_window_open (sat_gtk3_window_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 window open error");

    object->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    if (object->window != NULL)
    {
        g_signal_connect (object->window, "delete_event", G_CALLBACK (sat_gtk3_window_close_event), NULL);

        sat_status_set (&status, true, "");
    }


    return status;
}

void sat_gtk3_window_show (sat_gtk3_window_t *object)
{
    gtk_widget_show_all (object->window);
}

static void sat_gtk3_window_close_event (GtkWidget *window, void *data)
{
    (void) window;
    (void) data;

    gtk_main_quit ();
}
