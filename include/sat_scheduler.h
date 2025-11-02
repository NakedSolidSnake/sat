#ifndef SAT_SCHEDULER_H_
#define SAT_SCHEDULER_H_

#include <sat_status.h>
#include <sat_set.h>
#include <stdint.h>
#include <pthread.h>

typedef void (*sat_scheduler_handler_t) (void *object);

typedef struct 
{
    sat_set_t *set;
    uint16_t events_amount;
    pthread_t thread;
    bool running;
} sat_scheduler_t;

typedef enum
{
    sat_scheduler_mode_static,
    sat_scheduler_mode_dynamic,
} sat_scheduler_mode_t;

typedef enum
{
    sat_scheduler_type_periodic,
    sat_scheduler_type_one_shot,
} sat_scheduler_type_t;

typedef struct
{
    uint16_t event_amount;
    sat_scheduler_mode_t mode;
} sat_scheduler_args_t;

typedef struct
{
    void *object;
    char *name;
    sat_scheduler_handler_t handler;
    sat_scheduler_type_t type;
    uint64_t timeout;
    uint64_t last_update;
} sat_scheduler_event_t;

sat_status_t sat_scheduler_init (sat_scheduler_t *object);
sat_status_t sat_scheduler_open (sat_scheduler_t *object, sat_scheduler_args_t *args);
sat_status_t sat_scheduler_add_event (sat_scheduler_t *object, sat_scheduler_event_t *event);
sat_status_t sat_scheduler_start (sat_scheduler_t *object);
sat_status_t sat_scheduler_stop (sat_scheduler_t *object);
sat_status_t sat_scheduler_is_running (sat_scheduler_t *object);
sat_status_t sat_scheduler_get_amount (sat_scheduler_t *object, uint16_t *amount);
sat_status_t sat_scheduler_close (sat_scheduler_t *object);


#endif/* SAT_SCHEDULER_H_ */
