#include <sat_event.h>
#include <string.h>

sat_status_t sat_event_init (sat_event_dispatcher_t *const object)
{
    sat_status_return_on_null (object, "object pointer is NULL");

    memset (object, 0, sizeof (sat_event_dispatcher_t));

    sat_status_return_on_success ();
}

sat_status_t sat_event_observer_add (sat_event_dispatcher_t *const object, sat_event_t event, const sat_event_observer_t *const observer)
{
    sat_status_return_on_null (object, "object pointer is NULL");
    sat_status_return_on_null (observer, "observer pointer is NULL");
    sat_status_return_on_equals (object->amount, SAT_EVENT_OBSERVER_AMOUNT, "observer limit reached");

    object->map [object->amount].event = event;
    object->map [object->amount].observer = observer;
    object->map [object->amount].context = (void *)observer;

    object->amount ++;

    sat_status_return_on_success ();
}

sat_status_t sat_event_fire (const sat_event_dispatcher_t *const object, sat_event_t event, const void *const data)
{
    sat_status_return_on_null (object, "object pointer is NULL");

    for (uint8_t i = 0; i < object->amount; i++)
    {
        const sat_event_observer_map_t *const map = &object->map [i];

        if (event == map->event)
        {
            map->observer->base.handler (map->context, data);
        }
    }

    sat_status_return_on_success ();
}