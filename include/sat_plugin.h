#ifndef SAT_PLUGIN_H_
#define SAT_PLUGIN_H_

#include <sat_status.h>

typedef struct 
{
    void *handle;
} sat_plugin_t;

typedef struct 
{
    char *library_name;
} sat_plugin_args_t;

sat_status_t sat_plugin_open (sat_plugin_t *object, sat_plugin_args_t *args);
sat_status_t sat_plugin_load_method (sat_plugin_t *object, const char *name, void **method);
sat_status_t sat_plugin_close (sat_plugin_t *object);

#endif/* SAT_PLUGIN_H_ */
