#ifndef SAT_SET_H_
#define SAT_SET_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct sat_set_t sat_set_t;

typedef bool (*sat_set_compare_t) (void *element, void *param);
typedef bool (*sat_set_is_equal_t) (void *element, void *new_element);

typedef enum
{
    sat_set_mode_static,
    sat_set_mode_dynamic,
} sat_set_mode_t;

typedef struct
{
    uint32_t size;
    uint32_t object_size;
    sat_set_is_equal_t is_equal;
    sat_set_mode_t mode;
} sat_set_args_t;

sat_status_t sat_set_create (sat_set_t **object, sat_set_args_t *args);
sat_status_t sat_set_add (sat_set_t *object, void *data);
sat_status_t sat_set_update_by (sat_set_t *object, void *data, uint32_t index);
sat_status_t sat_set_remove_by (sat_set_t *object, uint32_t index);
sat_status_t sat_set_remove_by_parameter (sat_set_t *object, void *param, sat_set_compare_t compare ,void *data);
sat_status_t sat_set_get_object_by (sat_set_t *object, uint32_t index, void *data);
sat_status_t sat_set_get_object_by_parameter (sat_set_t *object, void *param, sat_set_compare_t compare ,void *data);
sat_status_t sat_set_get_size (sat_set_t *object, uint32_t *size);
sat_status_t sat_set_destroy (sat_set_t *object);

#endif/* SAT_SET_H_ */
