#include <sat_cache.h>
#include <string.h>
#include <stdlib.h>

sat_status_t sat_cache_init (sat_cache_t *const object)
{
    sat_status_return_on_null (object, "null object");

    memset (object, 0, sizeof (sat_cache_t));

    sat_status_return_on_success ();
}

sat_status_t sat_cache_open (sat_cache_t *const object, const sat_cache_args_t *const args)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (args, "null args");
    sat_status_return_on_equals (args->buffer_size, 0, "zero buffer size");

    object->data.size = args->buffer_size;
    object->data.buffer = calloc (1, sizeof (char) * args->buffer_size);
    sat_status_return_on_null (object->data.buffer, "buffer allocation failed");

    sat_status_return_on_success ();
}

sat_status_t sat_cache_store (sat_cache_t *const object, const void *const data, uint32_t size)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (data, "null data");
    sat_status_return_on_greater_than (size, object->data.size, "size exceeds buffer size");

    memset (object->data.buffer, 0, object->data.size);
    memcpy (object->data.buffer, data, size);

    object->is_cached = true;

    sat_status_return_on_success ();
}

sat_status_t sat_cache_restore (const sat_cache_t *const object, void *const data, uint32_t size)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (data, "null data");
    sat_status_return_on_greater_than (size, object->data.size, "size exceeds buffer size");

    memset (data, 0, size);
    memcpy (data, object->data.buffer, size);

    sat_status_return_on_success ();
}

sat_status_t sat_cache_is_cached (const sat_cache_t *const object)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_false (object->is_cached, "data is not cached");

    sat_status_return_on_success ();
}

sat_status_t sat_cache_clear (sat_cache_t *const object)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_false (object->is_cached, "data is not cached");

    memset (object->data.buffer, 0, object->data.size);

    object->is_cached = false;

    sat_status_return_on_success ();
}

sat_status_t sat_cache_close (sat_cache_t *const object)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (object->data.buffer, "buffer is already closed");

    free (object->data.buffer);
    object->data.buffer = NULL;

    sat_status_return_on_success ();
}
