#ifndef SAT_JSON_H_
#define SAT_JSON_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct sat_json_mapper_t sat_json_mapper_t;

typedef sat_json_mapper_t *(*sat_json_get_next_t) (void *object, uint16_t index);

typedef enum 
{
    sat_json_type_object,
    sat_json_type_array,
    sat_json_type_int,
    sat_json_type_double,
    sat_json_type_float,
    sat_json_type_string
} sat_json_type_t;

struct sat_json_mapper_t
{
    const char *token;
    void *data;
    uint16_t size;
    sat_json_type_t type;
    void *child;
    uint16_t fields;
    sat_json_get_next_t get_next;
};

typedef struct
{
    void *json;
    char *buffer;
    uint32_t size;
    sat_json_type_t type;
} sat_json_t;

typedef struct 
{
    char *buffer;
    uint32_t size;
} sat_json_args_t;

sat_status_t sat_json_init (sat_json_t *object);
sat_status_t sat_json_open (sat_json_t *object, sat_json_args_t *args);

sat_status_t sat_json_serialize_create_object (sat_json_t *object);
sat_status_t sat_json_serialize_create_array (sat_json_t *object);
sat_status_t sat_json_serialize_add (sat_json_t *object, sat_json_type_t type, const char *token, const void *const data);

sat_status_t sat_json_deserialize (sat_json_t *object, const char *json, sat_json_mapper_t *mapper, uint16_t fields);

sat_status_t sat_json_to_string (sat_json_t *object, char **json);
sat_status_t sat_json_string_to_buffer (sat_json_t *object, char *buffer, uint32_t size);

sat_status_t sat_json_buffer_clear (sat_json_t *object);

sat_status_t sat_json_close (sat_json_t *object);

#endif/* SAT_JSON_H_ */
