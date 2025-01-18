#ifndef SAT_GTK3_H_
#define SAT_GTK3_H_

#include <sat_gtk3_window.h>
#include <sat_gtk3_button.h>
#include <sat_gtk3_label.h>
#include <sat_gtk3_box.h>

typedef struct 
{
    sat_gtk3_window_t window;
    bool initialized;
} sat_gtk3_t;


typedef struct 
{
    int argc;
    char **argv;
    sat_gtk_window_event_on_close_t on_close;
    void *data;
} sat_gtk3_args_t;

sat_status_t sat_gtk3_init (sat_gtk3_t *object);
sat_status_t sat_gtk3_open (sat_gtk3_t *object, sat_gtk3_args_t *args);
sat_status_t sat_gtk3_run (sat_gtk3_t *object);
sat_status_t sat_gtk3_close (sat_gtk3_t *object);

sat_status_t sat_gtk3_add_widget (sat_gtk3_t *object, sat_gtk3_widget_t *widget);

#endif/* SAT_GTK3_H_ */
