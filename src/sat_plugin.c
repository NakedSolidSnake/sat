#include <sat_plugin.h>
#include <dlfcn.h>
#include <stdlib.h>

sat_status_t sat_plugin_open (sat_plugin_t *object, sat_plugin_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat plugin open error");

    if (object != NULL && args != NULL && args->library_name != NULL)
    {
        object->handle = dlopen (args->library_name, RTLD_LAZY);

        if (object->handle != NULL)
        {
            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_plugin_load_method (sat_plugin_t *object, const char *name, void **method)
{
    sat_status_t status = sat_status_set (&status, false, "sat plugin close error");

    if (object != NULL && object->handle != NULL && name != NULL && method != NULL)
    {
        *method = dlsym (object->handle, name);

        if (dlerror () == NULL)
        {
            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_plugin_close (sat_plugin_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat plugin close error");
    if (object != NULL && object->handle != NULL)
    {
        dlclose (object->handle);

        sat_status_set (&status, true, "");
    }

    return status;
}