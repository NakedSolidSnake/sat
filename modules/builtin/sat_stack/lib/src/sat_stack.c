#include <sat_stack.h>
#include <stdlib.h>
#include <string.h>

struct sat_stack_t
{
    uint32_t size;
    uint32_t object_size;
    uint32_t amount;
    void *buffer;
};

sat_status_t sat_stack_create (sat_stack_t **const object, uint32_t size, uint32_t object_size)
{
    sat_status_return_on_null (object, "null object pointer");
    sat_status_return_on_equals (size, 0, "size is zero");
    sat_status_return_on_equals (object_size, 0, "object size is zero");

    *object = calloc (1, sizeof (struct sat_stack_t));
    sat_status_return_on_null (*object, "memory allocation failed");

    (*object)->buffer = calloc (1, size * object_size);
    if ((*object)->buffer == NULL)
    {
        free (*object);
        *object = NULL;
        sat_status_return_on_failure ("buffer memory allocation failed");
    }

    (*object)->size = size;
    (*object)->object_size = object_size;

    sat_status_return_on_success ();
}

sat_status_t sat_stack_push (sat_stack_t *const object, const void *const data)
{
    sat_status_return_on_null (object, "null object pointer");
    sat_status_return_on_null (data, "null data pointer");
    sat_status_return_on_greater_than_or_equal (object->amount, object->size, "stack overflow");

    memcpy (object->buffer + (object->amount * object->object_size), data, object->object_size);
    object->amount ++;

    sat_status_return_on_success ();
}

sat_status_t sat_stack_pop (sat_stack_t *const object, void *const data)
{
    sat_status_return_on_null (object, "null object pointer");
    sat_status_return_on_null (data, "null data pointer");
    sat_status_return_on_equals (object->amount, 0, "stack underflow");

    memcpy (data, object->buffer + ((object->amount - 1) * object->object_size), object->object_size);

    object->amount --;

    sat_status_return_on_success ();
}

sat_status_t sat_stack_get_size (const sat_stack_t *const object, uint32_t *const size)
{
    sat_status_return_on_null (object, "null object pointer");
    sat_status_return_on_null (size, "null size pointer");

    *size = object->amount;

    sat_status_return_on_success ();
}

sat_status_t sat_stack_destroy (sat_stack_t *const object)
{
    sat_status_return_on_null (object, "null object pointer");
    sat_status_return_on_null (object->buffer, "null buffer pointer");

    free (object->buffer);
    free (object);

    sat_status_return_on_success ();
}