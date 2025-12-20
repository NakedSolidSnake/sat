#ifndef SAT_STACK_H_
#define SAT_STACK_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct sat_stack_t sat_stack_t;

sat_status_t sat_stack_create (sat_stack_t **const object, uint32_t size, uint32_t object_size);
sat_status_t sat_stack_push (sat_stack_t *const object, const void *const data);
sat_status_t sat_stack_pop (sat_stack_t *const object, void *const data);
sat_status_t sat_stack_get_size (const sat_stack_t *const object, uint32_t *const size);
sat_status_t sat_stack_destroy (sat_stack_t *const object);
#endif/* SAT_STACK_H_ */
