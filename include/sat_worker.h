#ifndef SAT_WORKER_H_
#define SAT_WORKER_H_

#include <sat_status.h>
#include <sat_queue.h>
#include <pthread.h>
#include <stdbool.h>

typedef void (*sat_worker_handler_t) (void *object);

typedef struct 
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_t *threads;
    uint8_t threads_amount;
    sat_queue_t *queue;
    uint32_t object_size;
    sat_worker_handler_t handler;
    bool running;
} sat_worker_t;

typedef struct 
{
    uint8_t pool_amount;
    uint32_t object_size;
    sat_worker_handler_t handler;
} sat_worker_args_t;

sat_status_t sat_worker_init (sat_worker_t *object);
sat_status_t sat_worker_open (sat_worker_t *object, sat_worker_args_t *args);
sat_status_t sat_worker_feed (sat_worker_t *object, void *data);
sat_status_t sat_worker_close (sat_worker_t *object);

#endif/* SAT_WORKER_H_ */
