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
};

sat_status_t sat_map_create (sat_map_t **object, uint32_t key_size, uint32_t value_size, uint32_t list_size)
{
    sat_status_t status = sat_status_set (&status, false, "sat map create error");

    if (object != NULL && key_size > 0 && value_size > 0 && list_size > 0)
    {
        sat_status_set (&status, false, "sat map allocation error");

        *object = calloc (1, sizeof (sat_map_t));

        if (*object != NULL)
        {
            (*object)->key_size = key_size;
            (*object)->value_size = value_size;
            (*object)->list_size = list_size;

            status = sat_array_create (&(*object)->array, list_size, sizeof (sat_map_item_t));
        }
    }

    return status;
}

sat_status_t sat_map_add (sat_map_t *object, void *key, void *value)
{
    sat_status_t status = sat_status_set (&status, false, "sat map add error");

    if (object != NULL && key != NULL && value != NULL)
    {
        sat_map_item_t item;

        item.key = calloc (1, object->key_size);
        item.value = calloc (1, object->value_size);

        memcpy (item.key, key, object->key_size);
        memcpy (item.value, value, object->value_size);

        status = sat_array_add (object->array, (void *)&item);
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

            free (item.key);
            free (item.value);
        }

        status = sat_array_destroy (object->array);
        free (object);
    }

    return status;
}