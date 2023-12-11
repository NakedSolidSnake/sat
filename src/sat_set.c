#include <sat_set.h>
#include <sat_array.h>
#include <string.h>
#include <stdlib.h>

struct sat_set_t
{
    sat_array_t *array;
    void *element;
    uint32_t object_size;
    sat_set_is_equal_t is_equal;
};

sat_status_t sat_set_create (sat_set_t **object, uint32_t size, uint32_t object_size, sat_set_is_equal_t is_equal)
{
    sat_status_t status = sat_status_set (&status, false, "sat set create error");

    if (object != NULL && size > 0 && object_size > 0 && is_equal != NULL)
    {
        *object = calloc (1, sizeof (struct sat_set_t));

        if (*object != NULL)
        {
            (*object)->element = calloc (1, object_size);
            (*object)->object_size = object_size;

            status = sat_array_create (&(*object)->array, size, object_size);
            if (sat_status_get_result (&status) == true)
            {
                (*object)->is_equal = is_equal;
            }

            else 
            {
                free ((*object)->element);
                free (*object);
            }
        }
    }

    return status;
}

sat_status_t sat_set_add (sat_set_t *object, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat set add error");

    if (object != NULL && data != NULL)
    {
        uint32_t amount = 0;
        uint32_t index;

        sat_array_get_size (object->array, &amount);

        for (index = 0; index < amount; index++)
        {
            memset (object->element, 0, object->object_size);

            sat_array_get_object_by (object->array, index, object->element);

            if (object->is_equal (object->element, data) == true)
            {
                break;
            }
        }

        if (index == amount)
        {
            status = sat_array_add (object->array, data);
        }
    }

    return status;
}

sat_status_t sat_set_update_by (sat_set_t *object, void *data, uint32_t index)
{
    sat_status_t status = sat_status_set (&status, false, "sat set update by error");

    if (object != NULL)
    {
        status = sat_array_update_by (object->array, data, index);
    }

    return status;
}

sat_status_t sat_set_remove_by (sat_set_t *object, uint32_t index)
{
    sat_status_t status = sat_status_set (&status, false, "sat set remove by error");

    if (object != NULL)
    {
        status = sat_array_remove_by (object->array, index);
    }

    return status;
}

sat_status_t sat_set_get_object_by (sat_set_t *object, uint32_t index, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat set get object by error");

    if (object != NULL)
    {
        status = sat_array_get_object_by (object->array, index, data);
    }

    return status;
}

sat_status_t sat_set_get_object_by_parameter (sat_set_t *object, void *param, sat_set_compare_t compare ,void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat set get object by parameter error");

    if (object != NULL)
    {
        status = sat_array_get_object_by_parameter (object->array, param, (sat_array_compare_t)compare, data);
    }

    return status;
}

sat_status_t sat_set_get_size (sat_set_t *object, uint32_t *size)
{
    sat_status_t status = sat_status_set (&status, false, "sat set get size error");

    if (object != NULL)
    {
        status = sat_array_get_size (object->array, size);
    }

    return status;
}

sat_status_t sat_set_destroy (sat_set_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat set destroy error");

    if (object != NULL && object->array != NULL)
    {
        status = sat_array_destroy (object->array);

        free ((*object).element);
        free (object);
    }

    return status;
}