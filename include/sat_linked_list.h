#ifndef SAT_LINKED_LIST_H_
#define SAT_LINKED_LIST_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct sat_linked_list_t sat_linked_list_t;

typedef bool (*sat_linked_list_compare_t) (void *element, void *data);
typedef void  (*sat_linked_list_print_t) (void *element);

sat_status_t sat_linked_list_create (sat_linked_list_t **object, uint32_t object_size);

sat_status_t sat_linked_list_insert (sat_linked_list_t *object, const void *element);

sat_status_t sat_linked_list_remove (sat_linked_list_t *object, sat_linked_list_compare_t compare, const void *param);

sat_status_t sat_linked_list_get (sat_linked_list_t *object, sat_linked_list_compare_t compare, const void *param, void *element);

sat_status_t sat_linked_list_get_size (sat_linked_list_t *object, uint32_t *size);

sat_status_t sat_linked_list_debug (sat_linked_list_t *object, sat_linked_list_print_t print);

sat_status_t sat_linked_list_destroy (sat_linked_list_t *object);

#endif/* SAT_LINKED_LIST_H_ */
 