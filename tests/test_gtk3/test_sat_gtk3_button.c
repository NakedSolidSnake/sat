#include <sat.h>
#include <assert.h>

int main (int argc, char *argv [])
{
    sat_gtk3_t gtk;
    

    sat_status_t status = sat_gtk3_init (&gtk);
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_open (&gtk, &(sat_gtk3_args_t){.argc = argc, .argv = argv});
    assert (sat_status_get_result (&status) == true);

    sat_gtk3_button_t button;
    sat_gtk3_button_create (&button, &(sat_gtk3_button_args_t){.name = "button"});
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_add_widget (&gtk, SAT_GTK3_WIDGET_CAST (button));
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_run (&gtk);
    assert (sat_status_get_result (&status) == true);

    status = sat_gtk3_close (&gtk);

    return 0;
}
