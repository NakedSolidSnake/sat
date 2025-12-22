#ifndef SAT_GTK3_WINDOW_H_
#define SAT_GTK3_WINDOW_H_

#include <sat_status.h>
#include <sat_gtk3_widget.h>

typedef void (*sat_gtk_window_event_on_close_t) (void *object, void *data);

typedef struct 
{
    void *window;
    sat_gtk_window_event_on_close_t on_close;
    void *data;
} sat_gtk3_window_t;

typedef struct 
{
    sat_gtk_window_event_on_close_t on_close;
    void *data;
} sat_gtk3_window_args_t;

sat_status_t sat_gtk3_window_open (sat_gtk3_window_t *object, sat_gtk3_window_args_t *args);
void sat_gtk3_window_show (sat_gtk3_window_t *object);
void sat_gtk3_window_add_widget (sat_gtk3_window_t *object, sat_gtk3_widget_t *widget);

#endif/* SAT_GTK3_WINDOW_H_ */
