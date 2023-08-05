#include <sat_event.h>
#include <string.h>

sat_status_t sat_event_init (sat_event_dispatcher_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat event init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_event_dispatcher_t));
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_event_observer_add (sat_event_dispatcher_t *object, sat_event_t event, sat_event_observer_t *observer)
{
    sat_status_t status = sat_status_set (&status, false, "sat event observer add error");

    if (object != NULL && observer != NULL && object->amount < SAT_EVENT_OBSERVER_AMOUNT)
    {
        object->map [object->amount].event = event;
        object->map [object->amount].observer = observer;
        object->map [object->amount].context = (void *)observer;

        object->amount ++;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_event_fire (sat_event_dispatcher_t *object, sat_event_t event, const void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat event fire error");

    if (object != NULL)
    {

        for (uint8_t i = 0; i < object->amount; i++)
        {

            sat_event_observer_map_t *map = &object->map [i];

            if (event == map->event)
            {
                map->observer->base.handler (map->context, data);
                sat_status_set (&status, true, "");
            }
        }        
    }

    return status;
}