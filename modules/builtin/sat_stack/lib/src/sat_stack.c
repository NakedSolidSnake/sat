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
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat stack create null object pointer");
            break;
        }

        if (size == 0)
        {
            sat_status_failure (&status, "sat stack create size is zero");
            break;
        }

        if (object_size == 0)
        {
            sat_status_failure (&status, "sat stack create object size is zero");
            break;
        }

        *object = calloc (1, sizeof (struct sat_stack_t));
        if (*object == NULL)
        {
            sat_status_failure (&status, "sat stack create memory allocation failed");
            break;
        }

        (*object)->buffer = calloc (1, size * object_size);
        if ((*object)->buffer == NULL)
        {
            free (*object);
            *object = NULL;

            sat_status_failure (&status, "sat stack create buffer memory allocation failed");
            break;
        }

        (*object)->size = size;
        (*object)->object_size = object_size;

    } while (false);

    return status;
}

sat_status_t sat_stack_push (sat_stack_t *const object, const void *const data)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat stack push null object");
            break;
        }

        if (data == NULL)
        {
            sat_status_set (&status, false, "sat stack push null data");
            break;
        }

        if (object->amount >= object->size)
        {
            sat_status_set (&status, false, "sat stack push stack overflow");
            break;
        }

        memcpy (object->buffer + (object->amount * object->object_size), data, object->object_size);
        object->amount ++;

    } while (false);

    return status;
}

sat_status_t sat_stack_pop (sat_stack_t *const object, void *const data)
{
    sat_status_t status = sat_status_success (&status);

    do 
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat stack pop null object");
            break;
        }

        if (data == NULL)
        {
            sat_status_set (&status, false, "sat stack pop null data");
            break;
        }

        if (object->amount == 0)
        {
            sat_status_set (&status, false, "sat stack pop stack underflow");
            break;
        }

        memcpy (data, object->buffer + ((object->amount - 1) * object->object_size), object->object_size);

        object->amount --;

    } while (false);

    return status;
}

sat_status_t sat_stack_get_size (const sat_stack_t *const object, uint32_t *const size)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat stack get size null object");
            break;
        }

        if (size == NULL)
        {
            sat_status_set (&status, false, "sat stack get size null size pointer");
            break;
        }

        *size = object->amount;

    } while (false);

    return status;
}

sat_status_t sat_stack_destroy (sat_stack_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat stack destroy null object");
            break;
        }

        if (object->buffer != NULL)
        {
            free (object->buffer);
        }

        free (object);

    } while (false);

    return status;
}