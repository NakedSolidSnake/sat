#include <sat_plugin.h>
#include <dlfcn.h>
#include <stdlib.h>

sat_status_t sat_plugin_open (sat_plugin_t *const object, const sat_plugin_args_t *const args)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat plugin open error: null object");
            break;
        }

        if (args == NULL)
        {
            sat_status_failure (&status, "sat plugin open error: null args");
            break;
        }

        if (args->library_name == NULL)
        {
            sat_status_failure (&status, "sat plugin open error: null library name");
            break;
        }

        object->handle = dlopen (args->library_name, RTLD_LAZY);
        if (object->handle == NULL)
        {
            sat_status_failure (&status, "sat plugin open error: failed to load library");
            break;
        }

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_plugin_load_method (sat_plugin_t *const object, const char *const name, void **const method)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat plugin load method error: null object");
            break;
        }

        if (object->handle == NULL)
        {
            sat_status_failure (&status, "sat plugin load method error: null handle");
            break;
        }

        if (name == NULL)
        {
            sat_status_failure (&status, "sat plugin load method error: null name");
            break;
        }

        if (method == NULL)
        {
            sat_status_failure (&status, "sat plugin load method error: null method");
            break;
        }

        *method = dlsym (object->handle, name);

        if (dlerror () == NULL)
        {
            sat_status_success (&status);            
        }
        
        else
        {
            sat_status_failure (&status, "sat plugin load method error: failed to load method");
        }

    } while (false);

    return status;
}

sat_status_t sat_plugin_close (sat_plugin_t *const object)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat plugin close error: null object");
            break;
        }

        if (object->handle == NULL)
        {
            sat_status_failure (&status, "sat plugin close error: null handle");
            break;
        }

        dlclose (object->handle);

        sat_status_success (&status);
        
    } while (false);

    return status;
}