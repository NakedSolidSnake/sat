#include <sat_gtk3_box.h>
#include <string.h>
#include <gtk/gtk.h>

sat_status_t sat_gtk3_box_create (sat_gtk3_box_t *object, sat_gtk3_box_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 box create error");

    if (object != NULL && args != NULL)
    {
        object->box.widget = gtk_box_new (args->type, args->space);

        if (object->box.widget != NULL)
        {
            object->initialized = true;

            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_gtk3_box_add_widget (sat_gtk3_box_t *object, sat_gtk3_widget_t *widget, sat_gtk3_box_add_properties_t properties)
{
    sat_status_t status = sat_status_set (&status, false, "sat gtk3 box add widget error");

    if (object != NULL && object->initialized == true && widget != NULL)
    {
        if (properties.add_type == sat_gtk3_box_add_start)
        {
            gtk_box_pack_start (object->box.widget,
                                widget->widget,
                                properties.expand,
                                properties.fill,
                                properties.padding);

        }

        else 
        {
            gtk_box_pack_end (object->box.widget,
                              widget->widget,
                              properties.expand,
                              properties.fill,
                              properties.padding);

        }

        sat_status_set (&status, true, "");
    }

    return status;
}