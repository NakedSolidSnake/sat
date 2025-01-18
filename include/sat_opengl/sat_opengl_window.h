#ifndef SAT_OPENGL_WINDOW_H_
#define SAT_OPENGL_WINDOW_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct 
{
    void *handle;
    uint16_t width;
    uint16_t height;
    const char *title;

} sat_opengl_window_t;

typedef struct 
{
    uint16_t width;
    uint16_t height;
    const char *title;
    
} sat_opengl_window_args_t;

sat_status_t sat_opengl_window_create (sat_opengl_window_t *object, sat_opengl_window_args_t *args);
bool sat_opengl_window_run (sat_opengl_window_t *object);
void sat_opengl_window_close (sat_opengl_window_t *object);

#endif/* SAT_OPENGL_WINDOW_H_ */
