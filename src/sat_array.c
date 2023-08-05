#include <sat_array.h>
#include <stdlib.h>
#include <string.h>

struct sat_array_t 
{
    uint32_t amount;
    uint32_t object_size;
    uint32_t size;
    uint8_t *buffer;
};

sat_status_t sat_array_create (sat_array_t **object, uint32_t size, uint32_t object_size)
{
    sat_status_t status = sat_status_set (&status, false, "sat array create error");

    if (object != NULL && size > 0 && object_size > 0)
    {
        sat_status_set (&status, false, "sat array instance error");

        *object = calloc (1, sizeof (struct sat_array_t));

        if (*object != NULL)
        {
            sat_status_set (&status, false, "sat array buffer error");

            (*object)->size = size;
            (*object)->object_size = object_size;

            (*object)->buffer = (uint8_t *) calloc (1, size * object_size);

            if ((*object)->buffer != NULL)
                sat_status_set (&status, true, "");

            else 
            {
                free ((*object)->buffer);
                free (*object);
            }

        }

    }

    return status;
}

sat_status_t sat_array_add (sat_array_t *object, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat array add error");

    if (object != NULL && data != NULL && object->amount < object->size)
    {
        memcpy (&object->buffer [object->amount * object->object_size], data, object->object_size);
        object->amount ++;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_array_update_by (sat_array_t *object, void *data, uint32_t index)
{
    sat_status_t status = sat_status_set (&status, false, "sat array update error");

    if (object != NULL && data != NULL && index < object->amount)
    {
        memcpy (&object->buffer [index * object->object_size], data, object->object_size);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_array_remove_by (sat_array_t *object, uint32_t index)
{
    sat_status_t status = sat_status_set (&status, false, "sat array remove error");

    if (object != NULL  && index < object->amount)
    {
        memset (&object->buffer [index * object->object_size], 0, object->object_size);

        for (uint32_t i = index; i < object->amount - 1; i++)
            memcpy (&object->buffer [i * object->object_size], 
                    &object->buffer [(i + 1) * object->object_size],
                    object->object_size);

        object->amount --;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_array_get_object_by (sat_array_t *object, uint32_t index, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat array create error");

    if (object != NULL && data != NULL && index < object->amount)
    {
        memset (data, 0, object->object_size);

        memcpy (data, &object->buffer [index * object->object_size], object->object_size);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_array_get_object_by_parameter (sat_array_t *object, void *param, sat_array_compare_t compare ,void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat array parameter error");

    if (object != NULL && data != NULL && compare != NULL && param != NULL)    
    {
        for (uint32_t i = 0; i < object->amount; i++)
        {
            if (compare (&object->buffer [i * object->object_size], param) == true)
            {
                memset (data, 0, object->object_size);

                memcpy (data, &object->buffer [i * object->object_size], object->object_size);

                sat_status_set (&status, true, "");

                break;
            }
        }
    }

    return status;
}

sat_status_t sat_array_get_size (sat_array_t *object, uint32_t *size)
{
    sat_status_t status = sat_status_set (&status, false, "sat array create error");

    if (object != NULL && size != NULL)
    {
        *size = object->amount;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_array_destroy (sat_array_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat array create error");

    if (object != NULL  && object->buffer != NULL)
    {
        free (object->buffer);
        free (object);

        sat_status_set (&status, true, "");
    }

    return status;
}