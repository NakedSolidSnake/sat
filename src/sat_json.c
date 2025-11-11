#include <sat_json.h>
#include <string.h>
#include <cJSON.h>
#include <cJSON_Utils.h>
#include <math.h>
#include <stdlib.h>

static bool sat_json_is_type_valid (sat_json_type_t type);
static sat_status_t sat_json_add_to_object (sat_json_t *object, sat_json_type_t type, const char *token, void *data);
static sat_status_t sat_json_add_to_array (sat_json_t *object, sat_json_type_t type, const char *token, void *data);
static sat_status_t sat_json_deserialize_items (cJSON *json, sat_json_mapper_t *mapper, uint16_t fields);
static sat_status_t sat_json_deserialize_array (cJSON *json, sat_json_mapper_t *mapper);
static sat_status_t sat_json_deserialize_primitives (cJSON *json, sat_json_mapper_t *mapper);


sat_status_t sat_json_init (sat_json_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat json init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_json_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_json_open (sat_json_t *object, sat_json_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat json open error");

    if (object != NULL && args != NULL)
    {
        object->buffer = args->buffer;
        object->size = args->size;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_json_serialize_create_object (sat_json_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat json serialize create object error");

    if (object != NULL)
    {
        object->json = cJSON_CreateObject ();
        object->type = sat_json_type_object;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_json_serialize_create_array (sat_json_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat json serialize create array error");

    if (object != NULL)
    {
        object->json = cJSON_CreateArray ();
        object->type = sat_json_type_array;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_json_serialize_add (sat_json_t *object, sat_json_type_t type, const char *token, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat json serialize add error");

    if (object != NULL && sat_json_is_type_valid (type) == true)    
    {
        switch (object->type)
        {
        case sat_json_type_object:
            status = sat_json_add_to_object (object, type, token, data);
            break;

        case sat_json_type_array:
            status = sat_json_add_to_array (object, type, token, data);
            break;
        
        default:
            break;
        }
    }

    return status;
}

sat_status_t sat_json_deserialize (sat_json_t *object, const char *json, sat_json_mapper_t *mapper, uint16_t fields)
{
    sat_status_t status = sat_status_set (&status, false, "sat json deserialize error");

    if (object != NULL && json != NULL && mapper != NULL && fields > 0)
    {
        cJSON *json_root = cJSON_Parse (json);

        if (json_root != NULL)
        {
            status = sat_json_deserialize_items (json_root, mapper, fields);

            cJSON_Delete (json_root);

            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_json_to_string (sat_json_t *object, char **json)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat json error: [object is NULL]");
            break;
        }

        if (json == NULL)
        {
            sat_status_set (&status, false, "sat json error: [json is NULL]");
            break;
        }

        char *string = cJSON_PrintUnformatted (object->json);
        if  (string == NULL)
        {
            sat_status_set (&status, false, "sat json error: [string is NULL]");
            break;
        }

        if (strlen (string) > object->size)
        {
            sat_status_set (&status, false, "sat json error: [string is too large]");
            cJSON_free (string);
            break;
        }

        memset (object->buffer, 0, object->size);
        strncpy (object->buffer, string, strlen (string));

        *json = object->buffer;

        cJSON_free (string);

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_json_string_to_buffer (sat_json_t *object, char *buffer, uint32_t size)
{
    sat_status_t status;
    char *string = NULL;

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat json error: [object is NULL]");
            break;
        }

        if (buffer == NULL)
        {
            sat_status_set (&status, false, "sat json error: [buffer is NULL]");
            break;
        }

        if (size == 0)
        {
            sat_status_set (&status, false, "sat json error: [size cannot be 0]");
            break;
        }

        status = sat_json_to_string (object, &string);
        if (sat_status_get_result (&status) == false )
        {
            sat_status_set (&status, false, "sat json error: [it wasn't possible to be string]");
            break;
        }

        if (strlen (string) > size)
        {
            sat_status_set (&status, false, "sat json error: [buffer is too small]");
            break;
        }

        memset (buffer, 0, size);
        strncpy (buffer, string, size - 1);

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_json_buffer_clear (sat_json_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat json buffer clear error");

    if (object != NULL)
    {
        memset (object->buffer, 0, object->size);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_json_close (sat_json_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat json close error");

    if (object != NULL)
    {
        cJSON_Delete (object->json);

        sat_status_set (&status, true, "");
    }

    return status;
}

static bool sat_json_is_type_valid (sat_json_type_t type)
{
    bool status = false;

    if (type >= sat_json_type_object && type <= sat_json_type_string)
    {
        status = true;
    }

    return status;
}

static sat_status_t sat_json_add_to_object (sat_json_t *object, sat_json_type_t type, const char *token, void *data)
{
    sat_status_t status = sat_status_set (&status, true, "");

    cJSON *value;

    switch (type)
    {
    case sat_json_type_int:
        value = cJSON_CreateNumber (*(int *)data);
        cJSON_AddItemToObject (object->json, token, value);
        break;

    case sat_json_type_float:
        value = cJSON_CreateNumber (*(float *)data);
        cJSON_AddItemToObject (object->json, token, value);
        break;

    case sat_json_type_double:
        value = cJSON_CreateNumber (*(double *)data);
        cJSON_AddItemToObject (object->json, token, value);
        break;

    case sat_json_type_string:
        value = cJSON_CreateString ((char *)data);
        cJSON_AddItemToObject (object->json, token, value);
        break;

    case sat_json_type_array:
    case sat_json_type_object:
        cJSON_AddItemToObject (object->json, token, data);
        break;
    
    default:
        sat_status_set (&status, false, "sat json add to object error");
        break;
    }

    return status;
}

static sat_status_t sat_json_add_to_array (sat_json_t *object, sat_json_type_t type, const char *token, void *data)
{
    sat_status_t status = sat_status_set (&status, true, "");

    switch (type)
    {
    case sat_json_type_int:
    case sat_json_type_float:
    case sat_json_type_double:
    case sat_json_type_string:
        cJSON_AddItemToArray (object->json, cJSON_CreateString (data));
        break;
        
    case sat_json_type_array:
    case sat_json_type_object:
        cJSON_AddItemToArray (object->json, (cJSON *)data);
        break;
    
    default:
        sat_status_set (&status, false, "sat json add to array error");
        break;
    }

    return status;
}

static sat_status_t sat_json_deserialize_items (cJSON *json, sat_json_mapper_t *mapper, uint16_t fields)
{
    sat_status_t status = sat_status_set (&status, false, "sat json deserialize items error");

    for (uint16_t i = 0; i < fields; i++)
    {
        cJSON *item = cJSON_GetObjectItem (json, mapper [i].token);
        if (item != NULL)
        {
            if (mapper [i].type == sat_json_type_object)
            {
                sat_json_deserialize_items (item, mapper [i].child, mapper [i].fields);
            }

            else if (mapper [i].type == sat_json_type_array)
            {
                sat_json_deserialize_array (item, mapper [i].child);
            }

            else 
            {
                sat_json_deserialize_primitives (item, &mapper [i]);
            }
        }

        else if (mapper [i].type == sat_json_type_array)
        {
            sat_json_deserialize_array (json, mapper [i].child);
        }
    }

    return status;
}

static sat_status_t sat_json_deserialize_array (cJSON *json, sat_json_mapper_t *mapper)
{
    sat_status_t status = sat_status_set (&status, false, "sat json deserialize array error");

    uint16_t i = 0;
    cJSON *data = cJSON_GetArrayItem (json, i);
    uint16_t fields = mapper [i].fields;

    while (data != NULL && fields > 0 && mapper->get_next != NULL)
    {
        sat_json_mapper_t *temp = mapper->get_next (mapper->data, i);

        if (temp != NULL)
        {
            if (temp->token != NULL && strlen (temp->token) > 0)
            {
                sat_json_deserialize_items (data, temp, fields);
            }

            else
                sat_json_deserialize_primitives (data, temp);

            free (temp);
        }

        data = cJSON_GetArrayItem (json, ++i);
    }

    if (i > 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_json_deserialize_primitives (cJSON *json, sat_json_mapper_t *mapper)
{
    sat_status_t status = sat_status_set (&status, false, "sat json deserialize primitives error");

    if (mapper->type == sat_json_type_int)
    {
        memcpy (mapper->data, &json->valueint, mapper->size);
        sat_status_set (&status, true, "");
    }

    else if (mapper->type == sat_json_type_double)
    {
        memcpy (mapper->data, &json->valuedouble, mapper->size);
        sat_status_set (&status, true, "");
    }

    else if (mapper->type == sat_json_type_string)
    {
        memset (mapper->data, 0, mapper->size);
        strncpy (mapper->data, json->valuestring, fmin (strlen (json->valuestring), mapper->size));
        sat_status_set (&status, true, "");
    }

    return status;
}