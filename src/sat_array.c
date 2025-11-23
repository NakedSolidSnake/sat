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

static void sat_array_configure_iterator (sat_array_t *object);


sat_status_t sat_array_create (sat_array_t **object, sat_array_args_t *args)
{
    sat_status_t status;

    do
    {
        status = sat_array_is_args_valid (args);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        if (object == NULL)
        {
            sat_status_set (&status, false, "sat array error: object is NULL");

            break;
        }

        sat_array_t *__object = calloc (1, sizeof (struct sat_array_t));
        if (__object == NULL)
        {
            sat_status_set (&status, false, "sat array error: memory allocation failed");

            break;
        }

        sat_array_set_context (__object, args);

        __object->buffer = (uint8_t *) calloc (1, __object->size * __object->object_size);
        if (__object->buffer == NULL)
        {
            sat_status_set (&status, false, "sat array error: buffer allocation failed");

            free (__object);
            break;
        }

        sat_array_configure_iterator (__object);

        __object->initialized = true;
        *object = __object;

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_array_add (sat_array_t *object, void *data)
{
    sat_status_t status;

    do
    {
        if (sat_array_is_initialized (object) == false)
        {
            sat_status_set (&status, false, "sat array error: array is not initialized");
            break;
        }

        if (data == NULL)
        {
            sat_status_set (&status, false, "sat array error: data is NULL");
            break;
        }

        if (object->mode == sat_array_mode_dynamic && object->amount == object->size)
        {
            status = sat_array_realloc (object);
            // in future register a callback to handle situations where
            // there is no more memory to allocate.
        }

        // Check if there is space to add the new element
        // if the array is static, we do not allow adding more elements than the size
        if (object->amount >= object->size)
        {
            sat_status_set (&status, false, "sat array error: array is full");
            break;
        }

        memcpy (&object->buffer [object->amount * object->object_size],
                data,
                object->object_size);

        object->amount ++;

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_array_update_by (sat_array_t *object, void *data, uint32_t index)
{
    sat_status_t status;

    do
    {
        if (sat_array_is_initialized (object) == false)
        {
            sat_status_set (&status, false, "sat array error: array is not initialized");
            break;
        }

        if (data == NULL)
        {
            sat_status_set (&status, false, "sat array error: data is NULL");
            break;
        }
        if (index >= object->amount)
        {
            sat_status_set (&status, false, "sat array error: index out of bounds");
            break;
        }

        memcpy (&object->buffer [index * object->object_size], data, object->object_size);

        sat_status_set (&status, true, "");

    } while (false);
    
    return status;
}

sat_status_t sat_array_remove_by (sat_array_t *object, uint32_t index)
{
    sat_status_t status = sat_status_set (&status, false, "sat array remove error");

    do
    {
        if (sat_array_is_initialized (object) == false)
        {
            sat_status_set (&status, false, "sat array error: array is not initialized");
            break;
        }

        if (index >= object->amount)
        {
            sat_status_set (&status, false, "sat array error: index out of bounds");
            break;
        }

        if (object->amount == 0)
        {
            sat_status_set (&status, false, "sat array error: no elements to remove");
            break;
        }

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

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_array_remove_by_parameter (sat_array_t *object, void *param, sat_array_compare_t compare ,void *data)
{
    sat_status_t status;

    do
    {
        if (sat_array_is_initialized (object) == false)
        {
            sat_status_set (&status, false, "sat array error: array is not initialized");
            break;
        }

        if (compare == NULL)
        {
            sat_status_set (&status, false, "sat array error: compare function is NULL");
            break;
        }

        if (param == NULL)
        {
            sat_status_set (&status, false, "sat array error: parameter is NULL");
            break;
        }

        sat_status_set (&status, false, "sat array error: object not found");

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

sat_status_t sat_array_get_object_by (sat_array_t *object, uint32_t index, void *data)
{
    sat_status_t status;

    do
    {
        if (sat_array_is_initialized (object) == false)
        {
            sat_status_set (&status, false, "sat array error: array is not initialized");
            break;
        }

        if (data == NULL)
        {
            sat_status_set (&status, false, "sat array error: data is NULL");
            break;
        }

        if (index >= object->amount)
        {
            sat_status_set (&status, false, "sat array error: index out of bounds");
            break;
        }

        memset (data, 0, object->object_size);

        memcpy (data, &object->buffer [index * object->object_size], object->object_size);

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_array_get_object_by_parameter (sat_array_t *object, void *param, sat_array_compare_t compare ,void *data)
{
    sat_status_t status;

    do
    {
        if (sat_array_is_initialized (object) == false)
        {
            sat_status_set (&status, false, "sat array error: array is not initialized");
            break;
        }

        if (data == NULL)
        {
            sat_status_set (&status, false, "sat array error: data is NULL");
            break;
        }

        if (compare == NULL)
        {
            sat_status_set (&status, false, "sat array error: compare function is NULL");
            break;
        }

        if (param == NULL)
        {
            sat_status_set (&status, false, "sat array error: parameter is NULL");
            break;
        }

        sat_status_set (&status, false, "sat array error: object not found");

        for (uint32_t i = 0; i < object->amount; i ++)
        {
            if (compare (&object->buffer [i * object->object_size], param) == true)
            {
                memset (data, 0, object->object_size);

                memcpy (data, &object->buffer [i * object->object_size], object->object_size);

                sat_status_set (&status, true, "");

                break;
            }
        }

    } while (false);

    return status;
}

sat_status_t sat_array_get_size (sat_array_t *object, uint32_t *size)
{
    sat_status_t status = sat_status_set (&status, false, "sat array get size error");

    do
    {
        if (sat_array_is_initialized (object) == false)
        {
            sat_status_set (&status, false, "sat array error: array is not initialized");
            break;
        }

        if (size == NULL)
        {
            sat_status_set (&status, false, "sat array error: size pointer is NULL");
            break;
        }

        *size = object->amount;

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_array_clear (sat_array_t *object)
{
    sat_status_t status;

    do
    {
        if (sat_array_is_initialized (object) == false)
        {
            sat_status_set (&status, false, "sat array error: array is not initialized");
            break;
        }

        memset (object->buffer, 0, object->object_size * object->size);

        object->amount = 0;

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_array_get_capacity (sat_array_t *object, uint32_t *capacity)
{
    sat_status_t status;

    do
    {
        if (sat_array_is_initialized (object) == false)
        {
            sat_status_set (&status, false, "sat array error: array is not initialized");
            break;
        }

        if (capacity == NULL)
        {
            sat_status_set (&status, false, "sat array error: capacity pointer is NULL");
            break;
        }

        // Return the current size of the array
        *capacity = object->size;

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_array_destroy (sat_array_t *object)
{
    sat_status_t status;

    do
    {
        if (sat_array_is_initialized (object) == false)
        {
            sat_status_set (&status, false, "sat array error: array is not initialized");
            break;
        }

        if (object->buffer == NULL)
        {
            sat_status_set (&status, false, "sat array error: buffer is NULL");
            break;
        }

        // Free the buffer and the object itself
        free (object->buffer);
        object->buffer = NULL;

        object->initialized = false;

        sat_status_set (&status, true, "");

        // Free the object itself
        free (object);
        object = NULL;
        
        sat_status_set (&status, true, "");

    } while (false);

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
    sat_status_t status;

    do
    {
        if (args == NULL)
        {
            sat_status_set (&status, false, "sat array error: args is NULL");
            break;
        }

        if (args->size == 0)
        {
            sat_status_set (&status, false, "sat array error: size is 0");
            break;
        }

        if (args->object_size == 0)
        {
            sat_status_set (&status, false, "sat array error: object size is 0");
            break;
        }

        if (args->mode != sat_array_mode_static && args->mode != sat_array_mode_dynamic)
        {
            sat_status_set (&status, false, "sat array error: invalid mode");
            break;
        }

        sat_status_set (&status, true, "");

    } while (false);

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
    sat_status_t status;
    
    do
    {
        uint8_t *__new = (uint8_t *) realloc (object->buffer,
                                          object->size * 2 * object->object_size);    
        if (__new == NULL)
        {
            sat_status_set (&status, false, "sat array error: memory reallocation failed");
            break;
        }

        // Update the size of the array
        object->size *= 2;

        // Update the buffer pointer to the new memory location
        object->buffer = __new;

        sat_status_set (&status, true, "");

    } while (false);

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

static void sat_array_configure_iterator (sat_array_t *object)
{
    object->base.object = object;
    object->base.get_amount = sat_array_get_amount;
    object->base.next = sat_array_next;
}