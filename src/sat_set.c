#include <sat_set.h>
#include <sat_array.h>
#include <string.h>
#include <stdlib.h>
#include <sat_iterator.h>

struct sat_set_t
{
    sat_iterator_base_t base;
    sat_array_t *array;
    void *element;
    uint32_t object_size;
    sat_set_is_equal_t is_equal;
    uint32_t size;
    sat_set_mode_t mode;
};

static void *sat_set_next (void *object, uint32_t index);
static uint32_t sat_set_get_amount (void *object);

static sat_status_t sat_set_is_args_valid (sat_set_args_t *args);
static void sat_set_set_context (sat_set_t *object, sat_set_args_t *args);
static sat_status_t sat_set_buffer_allocate (sat_set_t *object);

sat_status_t sat_set_create (sat_set_t **object, sat_set_args_t *args)
{
    sat_status_t status;

    do
    {
        status = sat_set_is_args_valid (args);
        if (sat_status_get_result (&status) == false)
            break;

        sat_status_set (&status, false, "sat set create error");
        if (object == NULL)
            break;
        
        sat_set_t *__object = calloc (1, sizeof (struct sat_set_t));
        if (__object == NULL)
            break;

        __object->element = calloc (1, args->object_size);
        if (__object->element == NULL)
        {
            free (__object);
            break;
        }

        sat_set_set_context (__object, args);

        status = sat_set_buffer_allocate (__object);
        if (sat_status_get_result (&status) == false)
        {
            free (__object->element);
            free (__object);

            break;
        }

        __object->base.object = __object;
        __object->base.get_amount = sat_set_get_amount;
        __object->base.next = sat_set_next;

        *object = __object;

        sat_status_set (&status, true, "");

    } while (false);

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

sat_status_t sat_set_remove_by_parameter (sat_set_t *object, void *param, sat_set_compare_t compare ,void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat set remove by parameter error");

    if (object != NULL)
    {
        status = sat_array_remove_by_parameter (object->array, param, (sat_array_compare_t)compare, data);
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

static sat_status_t sat_set_is_args_valid (sat_set_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat set is args valid error");

    if (args != NULL &&
        args->size > 0 &&
        args->object_size > 0 &&
        args->is_equal != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static void sat_set_set_context (sat_set_t *object, sat_set_args_t *args)
{
    object->object_size = args->object_size;
    object->is_equal = args->is_equal;
    object->mode = args->mode;
    object->size = args->size;
}

static sat_status_t sat_set_buffer_allocate (sat_set_t *object)
{
    return sat_array_create (&object->array, &(sat_array_args_t)
                                             {
                                                .size = object->size,
                                                .object_size = object->object_size,
                                                .mode = (sat_array_mode_t) object->mode
                                             });
}

static void *sat_set_next (void *object, uint32_t index)
{
    sat_set_t *set = (sat_set_t *) object;

    void *item = NULL;

    if (set->object_size > 0)
    {
        item = sat_array_get_reference_by (set->array, index);
    }

    return item;
}

static uint32_t sat_set_get_amount (void *object)
{
    sat_set_t *set = (sat_set_t *) object;

    uint32_t amount = 0;

    sat_array_get_size (set->array, &amount);

    return amount;
}