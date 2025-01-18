#ifndef SAT_EVENT_H_
#define SAT_EVENT_H_

#include <sat_status.h>
#include <stdint.h>

#define SAT_EVENT_OBSERVER_AMOUNT       10

typedef void (*sat_event_handler_t) (void *object, const void *data);
typedef uint16_t sat_event_t;

typedef struct 
{
    void *object;
    sat_event_handler_t handler;
} sat_event_base_t;

typedef struct 
{
    sat_event_base_t base;
} sat_event_observer_t;

typedef struct 
{
    sat_event_t event;
    sat_event_observer_t *observer;
    void *context;
} sat_event_observer_map_t;

typedef struct 
{
    sat_event_observer_map_t map [SAT_EVENT_OBSERVER_AMOUNT];
    uint8_t amount;
} sat_event_dispatcher_t;

sat_status_t sat_event_init (sat_event_dispatcher_t *object);
sat_status_t sat_event_observer_add (sat_event_dispatcher_t *object, sat_event_t event, sat_event_observer_t *observer);
sat_status_t sat_event_fire (sat_event_dispatcher_t *object, sat_event_t event, const void *data);


#endif/* SAT_EVENT_H_ */
