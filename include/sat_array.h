#ifndef SAT_ARRAY_H_
#define SAT_ARRAY_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct sat_array_t sat_array_t;

typedef bool (*sat_array_compare_t) (void *element, void *param);

typedef enum
{
    sat_array_mode_static,
    sat_array_mode_dynamic,
} sat_array_mode_t;

typedef struct
{
    uint32_t size;
    uint32_t object_size;
    sat_array_mode_t mode;
} sat_array_args_t;

sat_status_t sat_array_create (sat_array_t **object, sat_array_args_t *args);
sat_status_t sat_array_add (sat_array_t *object, void *data);
sat_status_t sat_array_update_by (sat_array_t *object, void *data, uint32_t index);
sat_status_t sat_array_remove_by (sat_array_t *object, uint32_t index);
sat_status_t sat_array_remove_by_parameter (sat_array_t *object, void *param, sat_array_compare_t compare ,void *data);
sat_status_t sat_array_get_object_by (sat_array_t *object, uint32_t index, void *data);
sat_status_t sat_array_get_object_by_parameter (sat_array_t *object, void *param, sat_array_compare_t compare ,void *data);
sat_status_t sat_array_get_size (sat_array_t *object, uint32_t *size);
sat_status_t sat_array_clear (sat_array_t *object);
sat_status_t sat_array_get_capacity (sat_array_t *object, uint32_t *capacity);
sat_status_t sat_array_destroy (sat_array_t *object);

void *sat_array_get_reference_by (sat_array_t *object, uint32_t index);

#endif/* SAT_ARRAY_H_ */
