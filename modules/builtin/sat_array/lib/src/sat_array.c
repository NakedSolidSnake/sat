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

    struct
    {
        sat_array_memory_notify_t on_increase;
        void *user;
    } notification;
};

static void *sat_array_next (const void *const object, const uint32_t index);
static uint32_t sat_array_get_amount (const void *const object);

static sat_status_t sat_array_is_initialized (const sat_array_t *const object);
static sat_status_t sat_array_is_args_valid (const sat_array_args_t *const args);

static void sat_array_set_context (sat_array_t *const object, const sat_array_args_t *const args);
static sat_status_t sat_array_realloc (sat_array_t *const object);

static void sat_array_configure_iterator (sat_array_t *const object);

sat_status_t sat_array_create (sat_array_t **const object, const sat_array_args_t *const args)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        sat_status_break_if_null (status, object, "sat array error: object pointer is NULL");

        status = sat_array_is_args_valid (args);
        sat_status_break_on_error (status);

        sat_array_t *__object = calloc (1, sizeof (struct sat_array_t));
        sat_status_break_if_null (status, __object, "sat array error: object allocation failed");

        sat_array_set_context (__object, args);

        __object->buffer = (uint8_t *) calloc (1, __object->size * __object->object_size);
        if (__object->buffer == NULL)
        {
            sat_status_failure (&status, "sat array error: buffer allocation failed");

            free (__object);
            break;
        }

        sat_array_configure_iterator (__object);

        __object->initialized = true;
        *object = __object;

    } while (false);

    return status;
}

sat_status_t sat_array_add (sat_array_t *const object, const void *const data)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);

        sat_status_break_if_null (status, data, "sat array error: data is NULL");

        if (object->mode == sat_array_mode_dynamic && object->amount == object->size)
        {
            status = sat_array_realloc (object);
            sat_status_break_on_error (status);
            // in future register a callback to handle situations where
            // there is no more memory to allocate.
            if (object->notification.on_increase != NULL)
            {
                object->notification.on_increase (object->notification.user, object->size);
            }
        }

        // Check if there is space to add the new element
        // if the array is static, we do not allow adding more elements than the size
        if (object->amount >= object->size)
        {
            sat_status_failure (&status, "sat array error: array is full");
            break;
        }

        memcpy (&object->buffer [object->amount * object->object_size],
                data,
                object->object_size);

        object->amount ++;

    } while (false);

    return status;
}

sat_status_t sat_array_update_by (sat_array_t *const object, const void *const data, const uint32_t index)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);

        sat_status_break_if_null (status, data, "sat array error: data is NULL");

        if (index >= object->amount)
        {
            sat_status_failure (&status, "sat array error: index out of bounds");
            break;
        }

        memcpy (&object->buffer [index * object->object_size], data, object->object_size);

    } while (false);
    
    return status;
}

sat_status_t sat_array_remove_by (sat_array_t *const object, uint32_t const index)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);

        if (index >= object->amount)
        {
            sat_status_failure (&status, "sat array error: index out of bounds");
            break;
        }

        sat_status_break_if_equals (status, object->amount, 0, "sat array error: no elements to remove");

        // Clear the element at the specified index
        memset (&object->buffer [index * object->object_size], 0, object->object_size);

        // Shift elements to the left
        for (uint32_t i = index; i < object->amount - 1; i++)
        {
            memcpy (&object->buffer [i * object->object_size], 
                    &object->buffer [(i + 1) * object->object_size],
                    object->object_size);
        }

        object->amount --;

    } while (false);

    return status;
}

sat_status_t sat_array_remove_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare , void *const data)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);

        sat_status_break_if_null (status, compare, "sat array error: compare is NULL");

        sat_status_break_if_null (status, param, "sat array error: parameter is NULL");


        sat_status_failure (&status, "sat array error: object not found");
        for (uint32_t i = 0; i < object->amount; i ++)
        {
            if (compare (&object->buffer [i * object->object_size], param) == true)
            {
                // Copy the data to the provided pointer
                if (data != NULL)
                {
                    memset (data, 0, object->object_size);
                    memcpy (data, &object->buffer [i * object->object_size], object->object_size);
                }

                sat_array_remove_by (object, i);

                sat_status_success (&status);

                break;
            }
        }

    } while (false);

    return status;
}

sat_status_t sat_array_get_object_by (const sat_array_t *const object, const uint32_t index, void *const data)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);

        sat_status_break_if_null (status, data, "sat array error: data is NULL");

        if (index >= object->amount)
        {
            sat_status_failure (&status, "sat array error: index out of bounds");
            break;
        }

        memcpy (data, &object->buffer [index * object->object_size], object->object_size);

    } while (false);

    return status;
}

sat_status_t sat_array_get_object_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare, void *const data)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);

        sat_status_break_if_null (status, data, "sat array error: data is NULL");

        sat_status_break_if_null (status, compare, "sat array error: compare function is NULL");

        sat_status_break_if_null (status, param, "sat array error: parameter is NULL");

        sat_status_failure (&status, "sat array error: object not found");
        for (uint32_t i = 0; i < object->amount; i ++)
        {
            if (compare (&object->buffer [i * object->object_size], param) == true)
            {
                memset (data, 0, object->object_size);

                memcpy (data, &object->buffer [i * object->object_size], object->object_size);

                sat_status_success (&status);

                break;
            }
        }

    } while (false);

    return status;
}

sat_status_t sat_array_get_object_ref_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare, void **const data)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);

        sat_status_break_if_null (status, data, "sat array error: data pointer is NULL");

        sat_status_break_if_null (status, compare, "sat array error: compare function is NULL");

        sat_status_break_if_null (status, param, "sat array error: parameter is NULL");

        sat_status_failure (&status, "sat array error: object not found");

        for (uint32_t i = 0; i < object->amount; i ++)
        {
            if (compare (&object->buffer [i * object->object_size], param) == true)
            {
                *data = &object->buffer [i * object->object_size];

                sat_status_success (&status);

                break;
            }
        }

    } while (false);

    return status;
}

sat_status_t sat_array_get_size (const sat_array_t *const object, uint32_t *const size)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);

        sat_status_break_if_null (status, size, "sat array error: size pointer is NULL");

        *size = object->amount;

    } while (false);

    return status;
}

sat_status_t sat_array_clear (sat_array_t *const object)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);

        memset (object->buffer, 0, object->object_size * object->size);

        object->amount = 0;

    } while (false);

    return status;
}

sat_status_t sat_array_get_capacity (const sat_array_t *const object, uint32_t *const capacity)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);

        sat_status_break_if_null (status, capacity, "sat array error: capacity pointer is NULL");

        // Return the current size of the array
        *capacity = object->size;

    } while (false);

    return status;
}

sat_status_t sat_array_destroy (sat_array_t *const object)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_initialized (object);
        sat_status_break_on_error (status);
        
        // Free the buffer and the object itself
        free (object->buffer);
        object->buffer = NULL;

        object->initialized = false;

        // Free the object itself
        free (object);

    } while (false);

    return status;
}

static sat_status_t sat_array_is_initialized (const sat_array_t *const object)
{
    sat_status_t status = sat_status_success (&status);
    do
    {
        sat_status_break_if_null (status, object, "sat array error: object is NULL");

        sat_status_break_if_false (status, object->initialized, "sat array error: object is not initialized");

    } while (false);

    return status;
}

static sat_status_t sat_array_is_args_valid (const sat_array_args_t *const args)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        sat_status_break_if_null (status, args, "sat array error: args is NULL");

        sat_status_break_if_equals (status, args->size, 0, "sat array error: size is 0");

        sat_status_break_if_equals (status, args->object_size, 0, "sat array error: object size is 0");

        if (args->mode != sat_array_mode_static && args->mode != sat_array_mode_dynamic)
        {
            sat_status_failure (&status, "sat array error: invalid mode");
            break;
        }

    } while (false);

    return status;
}

static void sat_array_set_context (sat_array_t *const object, const sat_array_args_t *const args)
{
    object->object_size = args->object_size;
    object->size = args->size;
    object->mode = args->mode;

    if (args->notification.on_increase != NULL)
    {
        object->notification.on_increase = args->notification.on_increase;
        object->notification.user = args->notification.user;
    }
}

static sat_status_t sat_array_realloc (sat_array_t *const object)
{
    sat_status_t status = sat_status_success (&status);
    
    do
    {
        uint8_t *__new = (uint8_t *) realloc (object->buffer,
                                          object->size * 2 * object->object_size);

        sat_status_break_if_null (status, __new, "sat array error: memory reallocation failed");

        // Update the size of the array
        object->size *= 2;

        // Update the buffer pointer to the new memory location
        object->buffer = __new;

    } while (false);

    return status;
}

static void *sat_array_next (const void *const object, const uint32_t index)
{
    sat_array_t *array = (sat_array_t *) object;

    void *item = NULL;

    if (array->object_size > 0)
    {
        item = &array->buffer [index * array->object_size];
    }

    return item;
}

static uint32_t sat_array_get_amount (const void *const object)
{
    sat_array_t *array = (sat_array_t *) object;
    uint32_t amount = 0;

    sat_array_get_size (array, &amount);

    return amount;
}

void *sat_array_get_reference_by (const sat_array_t *const object, const uint32_t index)
{
    void *item = NULL;
    sat_status_t status = sat_array_is_initialized (object);

    if (sat_status_get_result (&status) == true && index < object->amount)
    {
        item = &object->buffer [index * object->object_size];
    }

    return item;
}

static void sat_array_configure_iterator (sat_array_t *const object)
{
    object->base.object = object;
    object->base.get_amount = sat_array_get_amount;
    object->base.next = sat_array_next;
}