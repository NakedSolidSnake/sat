#ifndef SAT_GTK3_WINDOW_H_
#define SAT_GTK3_WINDOW_H_

#include <sat_status.h>

typedef struct 
{
    void *window;
} sat_gtk3_window_t;

sat_status_t sat_gtk3_window_open (sat_gtk3_window_t *object);
void sat_gtk3_window_show (sat_gtk3_window_t *object);

#endif/* SAT_GTK3_WINDOW_H_ */
