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
    sat_status_return_on_null (object, "object pointer is NULL");
    sat_status_return_on_error (sat_array_is_args_valid (args));

    sat_array_t *__object = calloc (1, sizeof (struct sat_array_t));
    sat_status_return_on_null (__object, "object allocation failed");

    sat_array_set_context (__object, args);

    __object->buffer = (uint8_t *) calloc (1, __object->size * __object->object_size);
    if (__object->buffer == NULL)
    {
        free (__object);
        sat_status_return_on_failure ("buffer allocation failed");
    }

    sat_array_configure_iterator (__object);

    __object->initialized = true;
    *object = __object;

    sat_status_return_on_success ();
}

sat_status_t sat_array_add (sat_array_t *const object, const void *const data)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_null (data, "data pointer is NULL");

    if (object->mode == sat_array_mode_dynamic && object->amount == object->size)
    {
        sat_status_return_on_error (sat_array_realloc (object));
        // in future register a callback to handle situations where
        // there is no more memory to allocate.
        if (object->notification.on_increase != NULL)
        {
            object->notification.on_increase (object->notification.user, object->size);
        }
    }

    sat_status_return_on_greater_than_or_equal (object->amount, object->size, "array is full");

    memcpy (&object->buffer [object->amount * object->object_size],
            data,
            object->object_size);

    object->amount ++;

    sat_status_return_on_success ();
}

sat_status_t sat_array_update_by (sat_array_t *const object, const void *const data, const uint32_t index)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_null (data, "data pointer is NULL");
    sat_status_return_on_greater_than_or_equal (index, object->amount, "index out of bounds");

    memcpy (&object->buffer [index * object->object_size], data, object->object_size);

    sat_status_return_on_success ();
}

sat_status_t sat_array_remove_by (sat_array_t *const object, uint32_t const index)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_greater_than_or_equal (index, object->amount, "index out of bounds");

    sat_status_return_on_equals (object->amount, 0, "no elements to remove");

    memset (&object->buffer [index * object->object_size], 0, object->object_size);

    for (uint32_t i = index; i < object->amount - 1; i++)
    {
        memcpy (&object->buffer [i * object->object_size], 
                &object->buffer [(i + 1) * object->object_size],
                object->object_size);
    }

    object->amount --;

    sat_status_return_on_success ();
}

sat_status_t sat_array_remove_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare , void *const data)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_null (compare, "compare function pointer is NULL");
    sat_status_return_on_null (param, "parameter pointer is NULL");

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

            return sat_array_remove_by (object, i);
        }
    }

    sat_status_return_on_failure ("object not found");
}

sat_status_t sat_array_get_object_by (const sat_array_t *const object, const uint32_t index, void *const data)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_null (data, "data is NULL");

    sat_status_return_on_greater_than_or_equal (index, object->amount, "index out of bounds");

    memcpy (data, &object->buffer [index * object->object_size], object->object_size);

    sat_status_return_on_success ();
}

sat_status_t sat_array_get_object_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare, void *const data)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_null (compare, "compare function pointer is NULL");
    sat_status_return_on_null (param, "parameter pointer is NULL");
    sat_status_return_on_null (data, "data pointer is NULL");

    for (uint32_t i = 0; i < object->amount; i ++)
    {
        sat_status_continue_on_false (compare (&object->buffer [i * object->object_size], param));

        memset (data, 0, object->object_size);

        memcpy (data, &object->buffer [i * object->object_size], object->object_size);

        sat_status_return_on_success ();
    }

    sat_status_return_on_failure ("object not found");
}

sat_status_t sat_array_get_object_ref_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare, void **const data)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_null (compare, "compare function pointer is NULL");
    sat_status_return_on_null (param, "parameter pointer is NULL");
    sat_status_return_on_null (data, "data pointer is NULL");

    for (uint32_t i = 0; i < object->amount; i ++)
    {
        sat_status_continue_on_false (compare (&object->buffer [i * object->object_size], param));

        *data = &object->buffer [i * object->object_size];

        sat_status_return_on_success ();
    }

    sat_status_return_on_failure ("object not found");
}


sat_status_t sat_array_get_size (const sat_array_t *const object, uint32_t *const size)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_null (size, "size pointer is NULL");

    *size = object->amount;

    sat_status_return_on_success ();
}

sat_status_t sat_array_clear (sat_array_t *const object)
{
    sat_status_return_on_error (sat_array_is_initialized (object));

    memset (object->buffer, 0, object->object_size * object->size);

    object->amount = 0;

    sat_status_return_on_success ();
}


sat_status_t sat_array_clone (const sat_array_t *const object, sat_array_t **const cloned)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_null (cloned, "cloned pointer is NULL");

    sat_array_args_t args =
    {
        .size = object->size,
        .object_size = object->object_size,
        .mode = object->mode,
        .notification =
        {
            .on_increase = NULL,
            .user = NULL
        }
    };

    sat_status_return_on_error (sat_array_create (cloned, &args));

    memcpy ((*cloned)->buffer, object->buffer, object->size * object->object_size);
    (*cloned)->amount = object->amount;

    sat_status_return_on_success ();
}

sat_status_t sat_array_get_capacity (const sat_array_t *const object, uint32_t *const capacity)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_null (capacity, "capacity pointer is NULL");

    *capacity = object->size;

    sat_status_return_on_success ();
}

sat_status_t sat_array_destroy (sat_array_t *const object)
{
    sat_status_return_on_error (sat_array_is_initialized (object));

    free (object->buffer);
    object->buffer = NULL;

    object->initialized = false;

    // Free the object itself
    free (object);
    
    sat_status_return_on_success ();
}

static sat_status_t sat_array_is_initialized (const sat_array_t *const object)
{
    sat_status_return_on_null (object, "object pointer is NULL");
    sat_status_return_on_false (object->initialized, "object is not initialized");

    sat_status_return_on_success ();
}

static sat_status_t sat_array_is_args_valid (const sat_array_args_t *const args)
{
    sat_status_return_on_null (args, "args pointer is NULL");
    sat_status_return_on_equals (args->size, 0, "size cannot be 0");
    sat_status_return_on_equals (args->object_size, 0, "object size cannot be 0");
    if (args->mode != sat_array_mode_static && args->mode != sat_array_mode_dynamic)
    {
        sat_status_return_on_failure ("invalid mode");
    }

    sat_status_return_on_success ();
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
    uint8_t *__new = (uint8_t *) realloc (object->buffer,
                                          object->size * 2 * object->object_size);
    sat_status_return_on_null (__new, "memory reallocation failed");

    object->size *= 2;

    // Update the buffer pointer to the new memory location
    object->buffer = __new;
    
    sat_status_return_on_success ();
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

sat_status_t sat_array_get_buffer (const sat_array_t *const object, sat_array_buffer_t *const buffer)
{
    sat_status_return_on_error (sat_array_is_initialized (object));
    sat_status_return_on_null (buffer, "buffer pointer is NULL");

    buffer->size = object->amount;
    buffer->data = object->buffer;

    sat_status_return_on_success ();
}

static void sat_array_configure_iterator (sat_array_t *const object)
{
    object->base.object = object;
    object->base.get_amount = sat_array_get_amount;
    object->base.next = sat_array_next;
}