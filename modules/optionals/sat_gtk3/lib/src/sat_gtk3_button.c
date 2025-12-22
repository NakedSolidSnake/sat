#include <sat_gtk3_button.h>
#include <string.h>
#include <gtk/gtk.h>

static void sat_gtk3_button_on_click (GtkWidget *widget, void *data);

static void sat_gtk3_button_set_events (sat_gtk3_button_t *object);

sat_status_t sat_gtk3_button_create (sat_gtk3_button_t *object, sat_gtk3_button_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 bitton create error");

    if (args != NULL)
    {
        if (strlen (args->name) > 0)
        {
            object->button.widget = gtk_button_new_with_label (args->name);
            sat_status_set (&status, true, "");    
        }

        else 
        {
            object->button.widget = gtk_button_new ();
            sat_status_set (&status, true, "");    
        }
    }

    sat_gtk3_button_set_events (object);

    if (args->on_click != NULL)
    {
        object->on_click = args->on_click;
    }

    if (args->data != NULL)
    {
        object->data = args->data;
    }

    return status;
}

static void sat_gtk3_button_on_click (GtkWidget *widget, void *data)
{
    (void) widget;
    
    sat_gtk3_button_t *button = (sat_gtk3_button_t *) data;

    if (button->on_click != NULL)
    {
        button->on_click (button, button->data);
    }
}

static void sat_gtk3_button_set_events (sat_gtk3_button_t *object)
{
    g_signal_connect (object->button.widget, "clicked", G_CALLBACK (sat_gtk3_button_on_click), object);
}