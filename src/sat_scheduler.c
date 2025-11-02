#include <sat_scheduler.h>
#include <string.h>
#include <unistd.h>
#include <sat_time.h>

static bool sat_scheduler_is_equal (void *element, void *new_element);
static bool sat_scheduler_is_event_valid (sat_scheduler_event_t *event);
static void *sat_scheduler_main_handler (void *parameters);

sat_status_t sat_scheduler_init (sat_scheduler_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat scheduler init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_scheduler_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_scheduler_open (sat_scheduler_t *object, sat_scheduler_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat scheduler open error");

    if (object != NULL && args != NULL && args->event_amount > 0)
    {
        object->events_amount = args->event_amount;

        status = sat_set_create (&object->set, &(sat_set_args_t)
                                                {
                                                    .size = object->events_amount,
                                                    .object_size = sizeof (sat_scheduler_event_t),
                                                    .is_equal = sat_scheduler_is_equal,
                                                    .mode = (sat_set_mode_t) args->mode
                                                });
    }

    return status;
}

sat_status_t sat_scheduler_add_event (sat_scheduler_t *object, sat_scheduler_event_t *event)
{
    sat_status_t status = sat_status_set (&status, false, "sat scheduler add event error");

    if (object != NULL && sat_scheduler_is_event_valid (event) == true)
    {
        status = sat_set_add (object->set, (void *)event);
    }

    return status;
}

sat_status_t sat_scheduler_start (sat_scheduler_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat scheduler start error");

    if (object != NULL)
    {
        object->running = true;

        pthread_create (&object->thread, NULL, sat_scheduler_main_handler, object);
        
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_scheduler_stop (sat_scheduler_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat scheduler stop error");

    if (object != NULL)
    {
        object->running = false;

        pthread_join (object->thread, NULL);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_scheduler_is_running (sat_scheduler_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat scheduler is running error");

    if (object != NULL && object->running == true)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_scheduler_get_amount (sat_scheduler_t *object, uint16_t *amount)
{
    sat_status_t status = sat_status_set (&status, false, "sat scheduler get amount error");

    if (object != NULL && amount != NULL)
    {
        sat_set_get_size (object->set, (uint32_t *)amount);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_scheduler_close (sat_scheduler_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat scheduler close error");

    if (object != NULL)
    {
        sat_scheduler_stop (object);

        sat_set_destroy (object->set);

        memset (object, 0, sizeof (sat_scheduler_t));
        
        sat_status_set (&status, true, "");
    }

    return status;
}

static bool sat_scheduler_is_equal (void *element, void *new_element)
{
    bool status = false;

    sat_scheduler_event_t *__element = (sat_scheduler_event_t *)element;
    sat_scheduler_event_t *__new_element = (sat_scheduler_event_t *)new_element;

    if (strcmp (__element->name, __new_element->name) == 0)
    {
        status = true;
    }

    return status;
}

static bool sat_scheduler_is_event_valid (sat_scheduler_event_t *event)
{
    bool status = false;

    if (event != NULL &&
        event->handler != NULL &&
        event->name != NULL &&
        strlen (event->name) > 0)
    {
        status = true;
    }

    return status;
}

static void *sat_scheduler_main_handler (void *parameters)
{
    sat_scheduler_t *object = (sat_scheduler_t *)parameters;
    uint32_t size;
    sat_scheduler_event_t event;
    uint64_t current;

    sat_set_get_size (object->set, &size);

    for (uint32_t i = 0; i < size; i++)
    {
        sat_set_get_object_by (object->set, i, &event);
        event.last_update = sat_time_get_epoch_now_ms ();
        sat_set_update_by (object->set, &event, i);
    }

    while (object->running)
    {
        current = sat_time_get_epoch_now_ms ();

        for (uint32_t i = 0; i < size; i++)
        {
            sat_set_get_object_by (object->set, i, &event);

            if (current > (event.timeout + event.last_update))
            {
                event.handler (event.object);

                if (event.type == sat_scheduler_type_one_shot)
                {
                    sat_set_remove_by (object->set, i);
                    sat_set_get_size (object->set, &size);

                    continue;
                }

                event.last_update = sat_time_get_epoch_now_ms ();
                sat_set_update_by (object->set, &event, i);
            }
        }

        usleep (10);
    }

    return NULL;
}