#include <sat_properties.h>
#include <ini.h>
#include <string.h>
#include <stdlib.h>

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

typedef void (*sat_properties_copy_value_t) (sat_properties_item_t *item, const char *value);

typedef struct
{
    sat_properties_type_t type;
    sat_properties_copy_value_t copy_value;
} sat_properties_type_handler_t;

static int sat_properties_handler (void *user, const char *section, const char *name, const char *value);
static bool sat_properties_check (sat_properties_item_t *item, const char *section, const char *name);
static void sat_properties_copy_value_to (sat_properties_item_t *item, const char *value);

static void sat_properties_copy_value_int (sat_properties_item_t *item, const char *value);
static void sat_properties_copy_value_decimal (sat_properties_item_t *item, const char *value);
static void sat_properties_copy_value_string (sat_properties_item_t *item, const char *value);

sat_status_t sat_properties_open (sat_properties_t *object, char *filename)
{
    sat_status_t status = sat_status_set (&status, false, "sat properties open error");

    if (object != NULL && filename != NULL)
    {
        object->filename = filename;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_properties_read (sat_properties_t *object, sat_properties_map_t *map)
{
    sat_status_t status = sat_status_set (&status, false, "sat properties read error");

    if (object != NULL && map != NULL && map->amount > 0)
    {
        if (ini_parse (object->filename, sat_properties_handler, map) == 0)
            sat_status_set (&status, true, "");
    }

    return status;
}

static int sat_properties_handler (void *user, const char *section, const char *name, const char *value)
{
    int status = 0;

    sat_properties_map_t *map = (sat_properties_map_t *)user;

    for (uint8_t i = 0; i < map->amount; i++)
    {
        sat_properties_item_t *item = &map->items [i];

        if (sat_properties_check (item, section, name) == true)
        {
            sat_properties_copy_value_to (item, value);

            status = 1;
            break;
        }
    }

    return status;
}

static bool sat_properties_check (sat_properties_item_t *item, const char *section, const char *name)
{
    bool status = false;

    if (MATCH (item->group, item->field))
    {
        status = true;
    }

    return status;
}

static void sat_properties_copy_value_to (sat_properties_item_t *item,const char *value)
{
    sat_properties_type_handler_t types [] = 
    {
        {.type = sat_properties_type_integer, .copy_value = sat_properties_copy_value_int},
        {.type = sat_properties_type_decimal, .copy_value = sat_properties_copy_value_decimal},
        {.type = sat_properties_type_string,  .copy_value = sat_properties_copy_value_string}
    };

    for (uint8_t i = 0; i < (sizeof (types) / sizeof (types [0])); i++)
    {
        if (item->type == types [i].type)
        {
            types [i].copy_value (item, value);
            break;
        }
    }
}

static void sat_properties_copy_value_int (sat_properties_item_t *item, const char *value)
{
    int __value = atoi (value);
    memcpy (item->data, &__value, item->size);
}

static void sat_properties_copy_value_decimal (sat_properties_item_t *item, const char *value)
{
    double __value = atof (value);
    memcpy (item->data, &__value, item->size);
}

static void sat_properties_copy_value_string (sat_properties_item_t *item, const char *value)
{
    memcpy (item->data, value, item->size);   
}