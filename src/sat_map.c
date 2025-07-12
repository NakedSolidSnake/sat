#include <sat_map.h>
#include <string.h>
#include <stdlib.h>
#include <sat_array.h>

typedef struct 
{
    void *key;
    void *value;
} sat_map_item_t;

struct sat_map_t
{
    uint32_t key_size;
    uint32_t value_size;
    uint32_t list_size;
    sat_array_t *array;
    sat_map_mode_t mode;
};

static sat_status_t sat_map_is_args_valid (sat_map_args_t *args);
static void sat_map_set_context (sat_map_t *object, sat_map_args_t *args);
static sat_status_t sat_map_buffer_allocate (sat_map_t *object);
static sat_status_t sat_map_alloc_item (sat_map_t *object, sat_map_item_t *item);
static sat_status_t sat_map_destroy_item (sat_map_item_t *item);

sat_status_t sat_map_create (sat_map_t **object, sat_map_args_t *args)
{
    sat_status_t status;

    do
    {
        status = sat_map_is_args_valid (args);
        if (sat_status_get_result (&status) == false)
            break;
        

        sat_status_set (&status, false, "sat map create error");
        if (object == NULL)
            break;

        sat_status_set (&status, false, "sat map allocation error");

        sat_map_t *__object = calloc (1, sizeof (sat_map_t));
        if (__object == NULL)
            break;

        sat_map_set_context (__object, args);

        status = sat_map_buffer_allocate (__object);
        if (sat_status_get_result (&status) == false)
        {
            free (__object);
            break;
        }

        *object = __object;

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_map_add (sat_map_t *object, void *key, void *value)
{
    sat_status_t status = sat_status_set (&status, false, "sat map add error");

    if (object != NULL && key != NULL && value != NULL)
    {
        sat_map_item_t item;

        status = sat_map_alloc_item (object, &item);
        if (sat_status_get_result (&status) == true)
        {
            memcpy (item.key, key, object->key_size);
            memcpy (item.value, value, object->value_size);

            status = sat_array_add (object->array, (void *)&item);
            if (sat_status_get_result (&status) == false)
            {
                sat_map_destroy_item (&item);
            }
        }
    }

    return status;
}

sat_status_t sat_map_remove (sat_map_t *object, void *key, sat_map_compare_t compare)
{
    sat_status_t status = sat_status_set (&status, false, "sat map remove error");

    if (object != NULL && key != NULL && compare != NULL)
    {
        uint32_t size;

        status = sat_array_get_size (object->array, &size);

        for (uint32_t i = 0; i < size; i++)
        {
            sat_map_item_t item;
            sat_array_get_object_by (object->array, i, &item);

            if (compare (item.key, (void *)key) == true)
            {
                free (item.key);
                free (item.value);

                status = sat_array_remove_by (object->array, i);
                break;
            }
        }
    }

    return status;
}

sat_status_t sat_map_get_size (sat_map_t *object, uint32_t *size)
{
    sat_status_t status = sat_status_set (&status, false, "sat map get size error");

    if (object != NULL && size != NULL)
    {
        status = sat_array_get_size (object->array, size);
    }

    return status;
}

sat_status_t sat_map_get_value_by (sat_map_t *object, const void *key, void *value, sat_map_compare_t compare)
{
    sat_status_t status = sat_status_set (&status, false, "sat map det value by error");

    if (object != NULL && key != NULL && value != NULL && compare != NULL)
    {
        uint32_t size;

        status = sat_array_get_size (object->array, &size);

        for (uint32_t i = 0; i < size; i++)
        {
            sat_map_item_t item;
            sat_array_get_object_by (object->array, i, &item);

            if (compare (item.key, (void *)key) == true)
            {
                memcpy (value, item.value, object->value_size);
                sat_status_set (&status, true, "");
                break;
            }
        }
    }

    return status;
}

sat_status_t sat_map_debug (sat_map_t *object, sat_map_print_t print)
{
    sat_status_t status = sat_status_set (&status, false, "sat map debug error");

    if (object != NULL && print != NULL)
    {
        uint32_t size;

        status = sat_array_get_size (object->array, &size);

        for (uint32_t i = 0; i < size; i++)
        {
            sat_map_item_t item;
            sat_array_get_object_by (object->array, i, &item);

            print (item.key, item.value);
        }

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_map_destroy (sat_map_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat map destroy error");

    if (object != NULL)
    {
        uint32_t size;

        status = sat_array_get_size (object->array, &size);

        for (uint32_t i = 0; i < size; i++)
        {
            sat_map_item_t item;
            sat_array_get_object_by (object->array, i, &item);

            status = sat_map_destroy_item (&item);
        }

        status = sat_array_destroy (object->array);
        free (object);
    }

    return status;
}

static sat_status_t sat_map_is_args_valid (sat_map_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat map is args valid error");

    if (args != NULL &&
        args->key_size > 0 &&
        args->value_size > 0 &&
        args->list_size > 0)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static void sat_map_set_context (sat_map_t *object, sat_map_args_t *args)
{
    object->key_size = args->key_size;
    object->value_size = args->value_size;
    object->list_size = args->list_size;
    object->mode = args->mode;
}

static sat_status_t sat_map_buffer_allocate (sat_map_t *object)
{
    return sat_array_create (&object->array, &(sat_array_args_t)
                                             {
                                                .size = object->list_size,
                                                .object_size = sizeof (sat_map_item_t),
                                                .mode = (sat_array_mode_t) object->mode
                                             });
}

static sat_status_t sat_map_alloc_item (sat_map_t *object, sat_map_item_t *item)
{
    sat_status_t status = sat_status_set (&status, false, "sat map alloc item error");

    if (object != NULL && item != NULL)
    {
        item->key = calloc (1, object->key_size);
        if (item->key == NULL)
            return sat_status_set (&status, false, "sat map key allocation error");

        item->value = calloc (1, object->value_size);
        if (item->value == NULL)
        {
            free(item->key);
            return sat_status_set (&status, false, "sat map value allocation error");
        }

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_map_destroy_item (sat_map_item_t *item)
{
    sat_status_t status = sat_status_set (&status, false, "sat map destroy item error");

    if (item != NULL && item->key != NULL && item->value != NULL)
    {
        free(item->key);

        free(item->value);

        sat_status_set (&status, true, "");
    }

    return status;
}