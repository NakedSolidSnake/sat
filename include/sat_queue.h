#ifndef SAT_QUEUE_H_
#define SAT_QUEUE_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct sat_queue_t sat_queue_t;

typedef void (*sat_queue_print_t) (void *element);

sat_status_t sat_queue_create (sat_queue_t **object, uint32_t object_size);
sat_status_t sat_queue_enqueue (sat_queue_t *object, void *data);
sat_status_t sat_queue_dequeue (sat_queue_t *object, void *data);
sat_status_t sat_queue_get_size (sat_queue_t *object, uint32_t *size);
sat_status_t sat_queue_debug (sat_queue_t *object, sat_queue_print_t print);
sat_status_t sat_queue_destroy (sat_queue_t *object);

#endif/* SAT_QUEUE_H_ */
