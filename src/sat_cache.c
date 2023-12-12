#include <sat_cache.h>
#include <string.h>
#include <stdlib.h>

sat_status_t sat_cache_init (sat_cache_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_cache_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_cache_open (sat_cache_t *object, sat_cache_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache open error");

    if (object != NULL && args != NULL && args->buffer_size > 0)
    {
        object->buffer_size = args->buffer_size;
        object->buffer = calloc (1, sizeof (char) * args->buffer_size);

        if (object->buffer != NULL)
            sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_cache_store (sat_cache_t *object, void *data, uint32_t size)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache store error");

    if (object != NULL && data != NULL && size <= object->buffer_size)
    {
        memset (object->buffer, 0, object->buffer_size);

        memcpy (object->buffer, data, size);

        object->is_cached = true;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_cache_restore (sat_cache_t *object, void *data, uint32_t size)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache restore error");

    if (object != NULL && size <= object->buffer_size && object->is_cached == true)
    {
        memset (data, 0, size);

        memcpy (data, object->buffer, size);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_cache_is_cached (sat_cache_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache is cached error");

    if (object != NULL && object->is_cached)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_cache_clear (sat_cache_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache clear error");

    if (object != NULL && object->is_cached == true)
    {
        memset (object->buffer, 0, object->buffer_size);

        object->is_cached = false;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_cache_close (sat_cache_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache close error");

    if (object != NULL && object->buffer != NULL)
    {
        free (object->buffer);

        sat_status_set (&status, true, "");
    }

    return status;
}