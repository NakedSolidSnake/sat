#include <sat_gtk3_button.h>
#include <string.h>
#include <gtk/gtk.h>

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

    return status;
}