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

sat_status_t sat_stack_create (sat_stack_t **object, uint32_t size, uint32_t object_size)
{
    sat_status_t status = sat_status_set (&status, false, "sat stack create error");

    if (object != NULL && size > 0 && object_size > 0)
    {
        sat_status_set (&status, false, "sat stack instance error");

        *object = calloc (1, sizeof (struct sat_stack_t));

        if (*object != NULL)
        {
            sat_status_set (&status, false, "sat stack buffer error");

            (*object)->size = size;
            (*object)->object_size = object_size;

            (*object)->buffer = calloc (1, size * object_size);

            if ((*object)->buffer != NULL)
                sat_status_set (&status, true, "");

            else
            {
                free (*object);
                *object = NULL;
            }
        }
    }

    return status;
}

sat_status_t sat_stack_push (sat_stack_t *object, const void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat stack push error");

    if (object != NULL && data != NULL && object->amount < object->size)
    {
        memcpy (object->buffer + (object->amount * object->object_size), data, object->object_size);
        object->amount ++;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_stack_pop (sat_stack_t *object, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat stack pop error");

    if (object != NULL && data != NULL && object->amount > 0)
    {
        memcpy (data, object->buffer + ((object->amount - 1) * object->object_size), object->object_size);

        object->amount --;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_stack_get_size (sat_stack_t *object, uint32_t *size)
{
    sat_status_t status = sat_status_set (&status, false, "sat stack get size error");

    if (object != NULL && size != NULL)
    {
        *size = object->amount;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_stack_destroy (sat_stack_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat stack destroy error");

    if (object != NULL && object->buffer != NULL)
    {
        free (object->buffer);
        free (object);
        
        sat_status_set (&status, true, "");
    }

    return status;
}