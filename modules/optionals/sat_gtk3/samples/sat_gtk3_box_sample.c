#include <sat.h>
#include <assert.h>

static void on_close (void *object, void *data);
static void on_click (void *object, void *data);

int main (int argc, char *argv [])
{
    sat_gtk3_t gtk;
    
    sat_status_t status = sat_gtk3_init (&gtk);
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_open (&gtk, &(sat_gtk3_args_t){.argc = argc, .argv = argv, .on_close = on_close});
    assert (sat_status_get_result (&status) == true);

    sat_gtk3_button_t button;
    sat_gtk3_button_create (&button, &(sat_gtk3_button_args_t)
                                     {
                                        .name = "button",
                                        .on_click = on_click,
                                        .data = &gtk
                                     });
    assert (sat_status_get_result (&status) == true);

    sat_gtk3_label_t label;

    status = sat_gtk3_label_create (&label, &(sat_gtk3_label_args_t){.name = "My Label"});
    assert (sat_status_get_result (&status) == true);

    sat_gtk3_box_t box;
    status = sat_gtk3_box_create (&box, &(sat_gtk3_box_args_t){.type = sat_gtk3_box_type_horizontal, .space = 5});
    assert (sat_status_get_result (&status) == true);

    sat_gtk3_box_add_properties_t properties = 
    {
        .add_type = sat_gtk3_box_add_start,
        .expand = false,
        .fill = false,
        .padding = 0
    };

    status = sat_gtk3_box_add_widget (&box, SAT_GTK3_WIDGET_CAST (label), properties);
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_box_add_widget (&box, SAT_GTK3_WIDGET_CAST (button), properties);
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_add_widget (&gtk, SAT_GTK3_WIDGET_CAST (box));
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_run (&gtk);
    assert (sat_status_get_result (&status) == true);

    return 0;
}

static void on_click (void *object, void *data)
{
    sat_gtk3_t *gtk = (sat_gtk3_t *)data;

    sat_gtk3_close (gtk);    
}

static void on_close (void *object, void *data)
{
    sat_gtk3_t *gtk = (sat_gtk3_t *)data;

    sat_gtk3_close (gtk);
}