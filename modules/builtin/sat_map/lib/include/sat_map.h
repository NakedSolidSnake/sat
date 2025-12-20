#ifndef SAT_MAP_H_
#define SAT_MAP_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct sat_map_t sat_map_t;

typedef bool (*sat_map_compare_t) (void *key, void *data);
typedef void (*sat_map_print_t) (const void *key, const void *value);

typedef enum
{
    sat_map_mode_static,
    sat_map_mode_dynamic,
} sat_map_mode_t;

typedef struct
{
    uint32_t key_size;
    uint32_t value_size;
    uint32_t list_size;
    sat_map_mode_t mode;
} sat_map_args_t;

sat_status_t sat_map_create (sat_map_t **object, sat_map_args_t *args);
sat_status_t sat_map_add (sat_map_t *object, void *key, void *value);
sat_status_t sat_map_remove (sat_map_t *object, void *key, sat_map_compare_t compare);
sat_status_t sat_map_get_size (sat_map_t *object, uint32_t *size);
sat_status_t sat_map_get_value_by (sat_map_t *object, const void *key, void *value, sat_map_compare_t compare);
sat_status_t sat_map_debug (sat_map_t *object, sat_map_print_t print);
sat_status_t sat_map_destroy (sat_map_t *object);

#endif/* SAT_MAP_H_ */
