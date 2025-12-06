#include <sat_cache.h>
#include <string.h>
#include <stdlib.h>

sat_status_t sat_cache_init (sat_cache_t *const object)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat cache init error: null object");
            break;
        }

        memset (object, 0, sizeof (sat_cache_t));

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_cache_open (sat_cache_t *const object, const sat_cache_args_t *const args)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat cache open error: null object");
            break;
        }

        if (args == NULL)
        {
            sat_status_failure (&status, "sat cache open error: null args");
            break;
        }

        if (args->buffer_size == 0)
        {
            sat_status_failure (&status, "sat cache open error: zero buffer size");
            break;
        }

        object->buffer_size = args->buffer_size;
        object->buffer = calloc (1, sizeof (char) * args->buffer_size);

        if (object->buffer == NULL)
        {
            sat_status_failure (&status, "sat cache open error: buffer allocation failed");
            break;
        }

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_cache_store (sat_cache_t *const object, const void *const data, uint32_t size)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat cache store error: null object");
            break;
        }

        if (data == NULL)
        {
            sat_status_failure (&status, "sat cache store error: null data");
            break;
        }

        if (size > object->buffer_size)
        {
            sat_status_failure (&status, "sat cache store error: size exceeds buffer size");
            break;
        }

        memset (object->buffer, 0, object->buffer_size);

        memcpy (object->buffer, data, size);

        object->is_cached = true;

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_cache_restore (const sat_cache_t *const object, void *const data, uint32_t size)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat cache restore error: null object");
            break;
        }

        if (data == NULL)
        {
            sat_status_failure (&status, "sat cache restore error: null data");
            break;
        }

        if (size > object->buffer_size)
        {
            sat_status_failure (&status, "sat cache restore error: size exceeds buffer size");
            break;
        }

        memset (data, 0, size);

        memcpy (data, object->buffer, size);

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_cache_is_cached (const sat_cache_t *const object)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat cache is cached error: null object");
            break;
        }

        if (object->is_cached == false)
        {
            sat_status_set (&status, false, "data is not cached");
            break;
        }

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_cache_clear (sat_cache_t *const object)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat cache clear error: null object");
            break;
        }

        if (object->is_cached == false)
        {
            sat_status_set (&status, true, "data is already not cached");
            break;
        }

        memset (object->buffer, 0, object->buffer_size);

        object->is_cached = false;

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_cache_close (sat_cache_t *const object)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat cache close error: null object");
            break;
        }

        if (object->buffer == NULL)
        {
            sat_status_set (&status, true, "buffer is already closed");
            break;
        }

        free (object->buffer);

        sat_status_success (&status);

    } while (false);

    return status;
}