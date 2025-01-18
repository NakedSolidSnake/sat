#include <sat_array.h>
#include <stdlib.h>
#include <string.h>
#include <sat_iterator.h>

struct sat_array_t 
{
    sat_iterator_base_t base;
    uint32_t amount;
    uint32_t object_size;
    uint32_t size;
    uint8_t *buffer;
    bool initialized;
    sat_array_mode_t mode;
};

static void *sat_array_next (void *object, uint32_t index);
static uint32_t sat_array_get_amount (void *object);

static bool sat_array_is_initialized (sat_array_t *object);
static sat_status_t sat_array_is_args_valid (sat_array_args_t *args);

static void sat_array_set_context (sat_array_t *object, sat_array_args_t *args);
static sat_status_t sat_array_realloc (sat_array_t *object);


sat_status_t sat_array_create (sat_array_t **object, sat_array_args_t *args)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_args_valid (args);
        if (sat_status_get_result (&status) == false)
            break;

        sat_status_set (&status, false, "sat array create error");

        if (object == NULL)
            break;

        sat_status_set (&status, false, "sat array instance error");

        sat_array_t *__object = calloc (1, sizeof (struct sat_array_t));
        if (__object == NULL)
            break;

        sat_array_set_context (__object, args);

        sat_status_set (&status, false, "sat array buffer error");

        __object->buffer = (uint8_t *) calloc (1, __object->size * __object->object_size);
        if (__object->buffer == NULL)
        {
            free (__object);
            break;
        }

        __object->base.object = __object;
        __object->base.get_amount = sat_array_get_amount;
        __object->base.next = sat_array_next;
        __object->initialized = true;

        *object = __object;

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_array_add (sat_array_t *object, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat array add error");

    if (sat_array_is_initialized (object) == true && data != NULL)
    {
        if (object->mode == sat_array_mode_dynamic && object->amount == object->size)
        {
            status = sat_array_realloc (object);
            // in future register a callback to handle situations where
            // there is no more memory to allocate.
        }

        if (object->amount < object->size)
        {
            memcpy (&object->buffer [object->amount * object->object_size],
                    data,
                    object->object_size);

            object->amount ++;

            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_array_update_by (sat_array_t *object, void *data, uint32_t index)
{
    sat_status_t status = sat_status_set (&status, false, "sat array update error");

    if (sat_array_is_initialized (object) == true && data != NULL && index < object->amount)
    {
        memcpy (&object->buffer [index * object->object_size], data, object->object_size);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_array_remove_by (sat_array_t *object, uint32_t index)
{
    sat_status_t status = sat_status_set (&status, false, "sat array remove error");

    if (sat_array_is_initialized (object) == true  && index < object->amount)
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
    sat_status_t status = sat_status_set (&status, false, "sat array get object by error");

    if (sat_array_is_initialized (object) == true && data != NULL && index < object->amount)
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

    if (sat_array_is_initialized (object) == true && data != NULL && compare != NULL && param != NULL)    
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
    sat_status_t status = sat_status_set (&status, false, "sat array get size error");

    if (sat_array_is_initialized (object) == true && size != NULL)
    {
        *size = object->amount;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_array_clear (sat_array_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat array clear error");

    if (sat_array_is_initialized (object) == true)
    {
        memset (object->buffer, 0, object->object_size * object->size);

        object->amount = 0;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_array_get_capacity (sat_array_t *object, uint32_t *capacity)
{
    sat_status_t status = sat_status_set (&status, false, "sat array get capacity error");

    if (sat_array_is_initialized (object) == true && capacity != NULL)
    {
        *capacity = object->size;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_array_destroy (sat_array_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat array create error");

    if (sat_array_is_initialized (object) == true && object->buffer != NULL)
    {
        free (object->buffer);
        free (object);

        sat_status_set (&status, true, "");
    }

    return status;
}

static bool sat_array_is_initialized (sat_array_t *object)
{
    bool status = false;

    if (object != NULL &&
        object->initialized == true)
    {
        status = true;
    }

    return status;
}

static sat_status_t sat_array_is_args_valid (sat_array_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat array args error");

    if (args != NULL &&
        args->size > 0 &&
        args->object_size > 0)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static void sat_array_set_context (sat_array_t *object, sat_array_args_t *args)
{
    object->object_size = args->object_size;
    object->size = args->size;
    object->mode = args->mode;
}

static sat_status_t sat_array_realloc (sat_array_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat array realloc error");

    uint8_t *__new = (uint8_t *) realloc (object->buffer,
                                          object->size * 2 * object->object_size);
    
    if (__new != NULL)
    {
        object->size *= 2;

        object->buffer = __new;

        sat_status_set (&status, true, "");
    }

    return status;
}

static void *sat_array_next (void *object, uint32_t index)
{
    sat_array_t *array = (sat_array_t *) object;

    void *item = NULL;

    if (array->object_size > 0)
    {
        item = &array->buffer [index * array->object_size];
    }

    return item;
}

static uint32_t sat_array_get_amount (void *object)
{
    sat_array_t *array = (sat_array_t *) object;
    uint32_t amount = 0;

    sat_array_get_size (array, &amount);

    return amount;
}

void *sat_array_get_reference_by (sat_array_t *object, uint32_t index)
{
    void *item = NULL;

    if (sat_array_is_initialized (object) == true && index < object->amount)
    {
        item = &object->buffer [index * object->object_size];
    }

    return item;
}