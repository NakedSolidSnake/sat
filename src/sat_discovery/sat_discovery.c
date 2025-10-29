#include <sat_discovery.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sat_discovery_interest.h>
#include <sat_iterator.h>
#include <sat_discovery_frame.h>

static sat_status_t sat_discovery_is_args_valid (sat_discovery_args_t *args);
static sat_status_t sat_discovery_server_setup (sat_discovery_t *object, sat_discovery_args_t *args);
static sat_status_t sat_discovery_scheduler_setup (sat_discovery_t *object);


static bool sat_discovery_is_equal (void *element, void *new_element)
{
    bool status = false;

    sat_discovery_interest_t *_element = (sat_discovery_interest_t *)element;
    sat_discovery_interest_t *_new_element = (sat_discovery_interest_t *)new_element;

    if (strcmp (_element->name, _new_element->name) == 0)
    {
        status = true;
    }

    return status;
}

static void sat_discovery_scan (void *object);
static void sat_discovery_announce (void *object);
static void sat_discovery_heartbeat (void *object);
static void sat_discovery_interest (void *object);

static void sat_discovery_on_receive (char *buffer, uint32_t *size, void *data)
{
    (void)buffer;
    (void)size;
    sat_discovery_t *service = (sat_discovery_t *)data;

    sat_discovery_frame_t frame;
    sat_discovery_frame_buffer_t frame_buffer;

    sat_status_t status = sat_discovery_frame_unpack (&frame, (sat_discovery_frame_buffer_t *)buffer);

    
    if (frame.header.type == sat_discovery_frame_type_announce)
    {
        printf ("Discovery announce received: %.*s\n", *size, buffer);
    }
    else if (frame.header.type == sat_discovery_frame_type_heartbeat)
    {
        printf ("Discovery heartbeat received: %.*s\n", *size, buffer);
    }
    else if (frame.header.type == sat_discovery_frame_type_interest)
    {
        printf ("Discovery interest received: %.*s\n", *size, buffer);
    }

    if (strcmp (buffer, service->service_name) == 0)
        return; // Ignore messages from itself

    // Handle received discovery message
    printf ("Discovery message received: %.*s\n", *size, buffer);
}

sat_status_t sat_discovery_init (sat_discovery_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_discovery_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_discovery_open (sat_discovery_t *object, sat_discovery_args_t *args)
{
    sat_status_t status;

    do
    {
        status = sat_discovery_is_args_valid (args);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_discovery_server_setup (object, args);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_discovery_scheduler_setup (object);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_set_create (&object->set, &(sat_set_args_t)
                                        {
                                            .size = 10,
                                            .object_size = sizeof (sat_discovery_interest_t),
                                            .is_equal = sat_discovery_is_equal,
                                            .mode = sat_set_mode_dynamic
                                        });
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        strncpy (object->service_name, args->name, SAT_DISCOVERY_SERVICE_NAME_MAX_LENGTH);
        strncpy (object->channel.service, args->channel.service, SAT_DISCOVERY_SERVICE_MAX_LENGTH);
        strncpy (object->channel.address, args->channel.address, SAT_DISCOVERY_ADDRESS_MAX_LENGTH);

    } while (false);

    return status;
}

sat_status_t sat_discovery_add_interest (sat_discovery_t *object, const char *name)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery add interest error");

    do
    {
        if (object == NULL || name == NULL)
        {
            sat_status_set (&status, false, "Invalid parameters");
            break;
        }

        sat_discovery_interest_t interest;
        status = sat_discovery_interest_create (&interest, name);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_set_add (object->set, &interest);

    } while (false);

    return status;
}

sat_status_t sat_discovery_start (sat_discovery_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery start error");

    if (object != NULL )
    {
        status = sat_scheduler_start (&object->scheduler);
    }

    return status;
}

sat_status_t sat_discovery_stop (sat_discovery_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery stop error");

    if (object != NULL )
    {
        // Stop the discovery process
        status = sat_scheduler_stop (&object->scheduler);
    }

    return status;
}

static sat_status_t sat_discovery_is_args_valid (sat_discovery_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery args invalid");

    if (args != NULL &&
        args->channel.service != NULL &&
        args->channel.address != NULL)      /* TODO create a function to check address */
    {
        // Validate the arguments
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_discovery_server_setup (sat_discovery_t *object, sat_discovery_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery server setup error");

    if (object != NULL)
    {
        static char buffer [1024] = {0};

        sat_udp_args_t _args = 
        {
            .type = sat_udp_type_server,
            .server = 
            {
                .service = args->channel.service,
                .buffer = buffer,
                .size = sizeof (buffer),
                .events = 
                {
                    .on_receive = sat_discovery_on_receive,
                    .on_send = NULL
                },
                .type = sat_udp_server_type_async,
                .mode = sat_udp_server_mode_multicast,
                .multicast_group = args->channel.address,
                .data = object
            }
        };

        status = sat_udp_open (&object->udp, &_args);
    }

    return status;
}

static sat_status_t sat_discovery_scheduler_setup (sat_discovery_t *object)
{
    sat_status_t status;

    do
    {
        status = sat_scheduler_open (&object->scheduler, &(sat_scheduler_args_t){.event_amount = 4});
        if (sat_status_get_result (&status) == false)
        {
            break;
        }
        status = sat_scheduler_add_event (&object->scheduler, &(sat_scheduler_event_t)
                                             {
                                                .name = "discovery announce",
                                                .object = object,
                                                .handler = (sat_scheduler_handler_t)sat_discovery_announce,
                                                .ran = false,
                                                .timeout = 500
                                             });
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_scheduler_add_event (&object->scheduler, &(sat_scheduler_event_t)
                                             {
                                                .name = "discovery scan",
                                                .object = object,
                                                .handler = (sat_scheduler_handler_t)sat_discovery_scan,
                                                .ran = false,
                                                .timeout = 100
                                             });
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_scheduler_add_event (&object->scheduler, &(sat_scheduler_event_t)
                                             {
                                                .name = "discovery heartbeat",
                                                .object = object,
                                                .handler = (sat_scheduler_handler_t)sat_discovery_heartbeat,
                                                .ran = false,
                                                .timeout = 1000
                                             });
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_scheduler_add_event (&object->scheduler, &(sat_scheduler_event_t)
                                             {
                                                .name = "discovery interest",
                                                .object = object,
                                                .handler = (sat_scheduler_handler_t)sat_discovery_interest,
                                                .ran = false,
                                                .timeout = 1000
                                             });

    } while (false);

    return status;
}

static void sat_discovery_scan (void *object)
{
    sat_discovery_t *discovery = (sat_discovery_t *)object;

    sat_udp_run (&discovery->udp);
}

static void sat_discovery_announce (void *object)
{
    sat_discovery_t *discovery = (sat_discovery_t *)object;

    sat_udp_send (&discovery->udp, discovery->service_name, strlen (discovery->service_name), &(sat_udp_destination_t)
                                                                          {
                                                                            .hostname = discovery->channel.address,
                                                                            .service = discovery->channel.service
                                                                          });
}

static void sat_discovery_heartbeat (void *object)
{
    sat_discovery_t *discovery = (sat_discovery_t *)object;




}

static void sat_discovery_interest (void *object)
{
    sat_discovery_t *discovery = (sat_discovery_t *)object;

    sat_iterator_t iterator;

    sat_status_t status = sat_iterator_open (&iterator, (sat_iterator_base_t *)discovery->set);
    if (sat_status_get_result (&status) == true)
    {
        sat_discovery_interest_t *interest = sat_iterator_next (&iterator);
        while (interest != NULL)
        {
            // Send interest message
            sat_udp_send (&discovery->udp, interest->name, strlen (interest->name), &(sat_udp_destination_t)
                                                                              {
                                                                                .hostname = discovery->channel.address,
                                                                                .service = discovery->channel.service
                                                                              });
            interest = sat_iterator_next (&iterator);
        }
    }
}