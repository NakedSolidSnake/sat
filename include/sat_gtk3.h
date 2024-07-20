#ifndef SAT_GTK3_H_
#define SAT_GTK3_H_

#include <sat_gtk3_window.h>

typedef struct 
{
    sat_gtk3_window_t window;
    bool initialized;
} sat_gtk3_t;


typedef struct 
{
    int argc;
    char **argv;
} sat_gtk3_args_t;

sat_status_t sat_gtk3_init (sat_gtk3_t *object);
sat_status_t sat_gtk3_open (sat_gtk3_t *object, sat_gtk3_args_t *args);
sat_status_t sat_gtk3_run (sat_gtk3_t *object);
sat_status_t sat_gtk3_close (sat_gtk3_t *object);

#endif/* SAT_GTK3_H_ */
