#ifndef SAT_GTK3_LABEL_H_
#define SAT_GTK3_LABEL_H_

#include <sat_status.h>
#include <sat_gtk3_widget.h>

typedef struct 
{
    sat_gtk3_widget_t label;
    bool initialized;
} sat_gtk3_label_t;

typedef struct 
{
    char *name;
} sat_gtk3_label_args_t;

sat_status_t sat_gtk3_label_create (sat_gtk3_label_t *object, sat_gtk3_label_args_t *args);
sat_status_t sat_gtk3_label_set_name (sat_gtk3_label_t *object, const char *text);
sat_status_t sat_gtk3_label_get_name (sat_gtk3_label_t *object, const char **text);

#endif/* SAT_GTK3_LABEL_H_ */
