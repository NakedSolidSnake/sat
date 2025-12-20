#include <sat_scheduler.h>
#include <string.h>
#include <unistd.h>
#include <sat_time.h>

static bool sat_scheduler_is_equal (const void *const element, const void *const new_element);
static bool sat_scheduler_is_event_valid (const sat_scheduler_event_t *const event);
static void *sat_scheduler_main_handler (void *const context);

sat_status_t sat_scheduler_init (sat_scheduler_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat scheduler init error: null object");
            break;
        }

        memset (object, 0, sizeof (sat_scheduler_t));

    } while (false);

        return status;
}

sat_status_t sat_scheduler_open (sat_scheduler_t *const object, const sat_scheduler_args_t *const args)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            status = sat_status_set (&status, false, "sat scheduler open error: null object");
            break;
        }

        if (args == NULL)
        {
            status = sat_status_set (&status, false, "sat scheduler open error: null args");
            break;
        }

        if (args->event_amount == 0)
        {
            status = sat_status_set (&status, false, "sat scheduler open error: zero event amount");
            break;
        }

        status = sat_set_create (&object->set, &(sat_set_args_t)
                                                {
                                                    .size = args->event_amount,
                                                    .object_size = sizeof (sat_scheduler_event_t),
                                                    .is_equal = sat_scheduler_is_equal,
                                                    .mode = (sat_set_mode_t) args->mode
                                                });
        sat_status_break_on_error (status);

        object->events_amount = args->event_amount;

    } while (false);

    return status;
}

sat_status_t sat_scheduler_add_event (sat_scheduler_t *const object, const sat_scheduler_event_t *const event)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            status = sat_status_set (&status, false, "sat scheduler add event error: null object");
            break;
        }

        if (sat_scheduler_is_event_valid (event) == false)
        {
            status = sat_status_set (&status, false, "sat scheduler add event error: null event");
            break;
        }

        status = sat_set_add (object->set, (void *)event);

    } while (false);

    return status;
}

sat_status_t sat_scheduler_start (sat_scheduler_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    if (object == NULL)
    {
        status = sat_status_set (&status, false, "sat scheduler start error: null object");
        return status;
    }
   
    object->running = true;

    pthread_create (&object->thread, NULL, sat_scheduler_main_handler, object);

    return status;
}

sat_status_t sat_scheduler_stop (sat_scheduler_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    if (object == NULL)
    {
        status = sat_status_set (&status, false, "sat scheduler start error: null object");
        return status;
    }

    object->running = false;

    pthread_join (object->thread, NULL);

    return status;
}

sat_status_t sat_scheduler_is_running (sat_scheduler_t *object)
{
    sat_status_t status = sat_status_success (&status);
    
    do
    {
        if (object == NULL)
        {
            status = sat_status_set (&status, false, "sat scheduler is running error: null object");
            break;
        }

        if (object->running == false)
        {
            sat_status_set (&status, false, "sat scheduler is running error: not running");
            break;
        }

    } while (false);

    return status;
}

sat_status_t sat_scheduler_get_amount (const sat_scheduler_t *const object, uint16_t *const amount)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            status = sat_status_set (&status, false, "sat scheduler get amount error: null object");
            break;
        }

        if (amount == NULL)
        {
            status = sat_status_set (&status, false, "sat scheduler get amount error: null amount");
            break;
        }

        sat_set_get_size (object->set, (uint32_t *)amount);

    } while (false);

    return status;
}

sat_status_t sat_scheduler_close (sat_scheduler_t *object)
{
    sat_status_t status = sat_status_success (&status);

    if (object == NULL)
    {
        status = sat_status_set (&status, false, "sat scheduler close error: null object");
        return status;
    }

    sat_scheduler_stop (object);

    sat_set_destroy (object->set);

    memset (object, 0, sizeof (sat_scheduler_t));

    return status;
}

static bool sat_scheduler_is_equal (const void *const element, const void *const new_element)
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

static bool sat_scheduler_is_event_valid (const sat_scheduler_event_t *const event)
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

static void *sat_scheduler_main_handler (void *const context)
{
    sat_scheduler_t *const object = (sat_scheduler_t *const)context;
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