#include <stdio.h>
#include <sat.h>
#include <string.h>
#include <assert.h>

typedef enum 
{
    print_event,
    compare_event
} event_t;

typedef struct 
{
    sat_event_observer_t observer;
} print_observer_t;

typedef struct 
{
    sat_event_observer_t observer;
} compare_observer_t;

typedef struct 
{
    char *message_1;
    char *message_2;
} value_t;

sat_status_t print_observer_init (print_observer_t *object);
static void print_data (void *object, const void *data);

sat_status_t compare_observer_init (compare_observer_t *object);
static void compare_data (void *object, const void *data);

int main (int argc, char *argv[])
{
    sat_event_dispatcher_t dispatcher;
    print_observer_t print;
    compare_observer_t compare;

    sat_status_t status = sat_event_init (&dispatcher);
    assert (sat_status_get_result (&status) == true);

    status = compare_observer_init (&compare);
    assert (sat_status_get_result (&status) == true);

    status = print_observer_init (&print);
    assert (sat_status_get_result (&status) == true);

    status = sat_event_observer_add (&dispatcher, print_event, (sat_event_observer_t *)&print);
    assert (sat_status_get_result (&status) == true);

    status = sat_event_observer_add (&dispatcher, compare_event, (sat_event_observer_t *)&compare);
    assert (sat_status_get_result (&status) == true);

    value_t value = 
    {
        .message_1 = "Text 1",
        .message_2 = "Text 2",
    };

    status = sat_event_fire (&dispatcher, print_event, "Hello World");
    assert (sat_status_get_result (&status) == true);

    status = sat_event_fire (&dispatcher, compare_event, &value);
    assert (sat_status_get_result (&status) == true);

    return 0;
}

sat_status_t print_observer_init (print_observer_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "print init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (print_observer_t));
        object->observer.base.handler = print_data;
        sat_status_set (&status, true, "");
    }

    return status;
}

static void print_data (void *object, const void *data)
{
    (void)object;

    printf ("%s\n", (char *)data);
}

sat_status_t compare_observer_init (compare_observer_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "compare init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (compare_observer_t));
        object->observer.base.handler = compare_data;
        sat_status_set (&status, true, "");
    }

    return status;
}

static void compare_data (void *object, const void *data)
{
    value_t *value = (value_t *)data;

    if (strcmp (value->message_1, value->message_2) == 0)
        printf ("%s\n", "Equals");

    else 
        printf ("%s\n", "Not Equals");
}