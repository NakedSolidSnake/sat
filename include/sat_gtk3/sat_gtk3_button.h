#ifndef SAT_GTK3_BUTTON_H_
#define SAT_GTK3_BUTTON_H_

#include <sat_status.h>
#include <sat_gtk3_widget.h>

typedef void (*sat_gtk3_button_on_click_t) (void *object, void *data);

typedef struct 
{
    sat_gtk3_widget_t button;
    sat_gtk3_button_on_click_t on_click;
    void *data;
} sat_gtk3_button_t;

typedef struct 
{
    char *name;
    sat_gtk3_button_on_click_t on_click;
    void *data;
} sat_gtk3_button_args_t;

sat_status_t sat_gtk3_button_create (sat_gtk3_button_t *object, sat_gtk3_button_args_t *args);

#endif/* SAT_GTK3_BUTTON_H_ */
