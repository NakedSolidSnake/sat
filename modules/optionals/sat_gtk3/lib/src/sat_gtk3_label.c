#include <sat_gtk3_label.h>
#include <string.h>
#include <gtk/gtk.h>

sat_status_t sat_gtk3_label_create (sat_gtk3_label_t *object, sat_gtk3_label_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 label create error");

    if (object != NULL && args != NULL && args->name != NULL)
    {
        object->label.widget = gtk_label_new (args->name);

        if (object->label.widget != NULL)
        {
            object->initialized = true;

            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_gtk3_label_set_name (sat_gtk3_label_t *object, const char *text)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 label set text error");

    if (object != NULL && text != NULL)
    {
        gtk_label_set_text (object->label.widget, text);    

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_gtk3_label_get_name (sat_gtk3_label_t *object, const char **text)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 label get text error");

    if (object != NULL && text != NULL)
    {
        *text = gtk_label_get_text (object->label.widget);    

        sat_status_set (&status, true, "");
    }

    return status;
}