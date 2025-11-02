#include <sat_discovery.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sat_discovery_interest.h>
#include <sat_iterator.h>
#include <sat_discovery_frame.h>
#include <sat_discovery_node.h>
#include <sat_time.h>
#include <sat_discovery_services.h>
#include <sat_discovery_handle_frames.h>
#include <unistd.h>


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

static void sat_discovery_on_receive (char *buffer, uint32_t *size, void *data)
{
    (void)size;
    sat_discovery_t *service = (sat_discovery_t *)data;

    sat_discovery_frame_t frame;

    sat_discovery_frame_unpack (&frame, (uint8_t *)buffer);

    // sat_discovery_frame_print (&frame);

    if (sat_uuid_compare_bin (frame.header.uuid, service->uuid) == true )
    {
        return; // Ignore messages from itself
    }

    // Ignores the same service announcements    
    if (frame.header.type == sat_discovery_frame_type_announce &&
        strcmp (frame.payload.announce.service_name, service->service_name) != 0)
    {
        sat_discovery_handle_frame_announce (service, frame);
    }
    else if (frame.header.type == sat_discovery_frame_type_heartbeat)
    {
        sat_discovery_handle_frame_heartbeat (service, frame);
    }
    else if (frame.header.type == sat_discovery_frame_type_interest)
    {
        sat_discovery_handle_frame_interest (service, frame);
    }

     else if (frame.header.type == sat_discovery_frame_type_vanish)
    {
        sat_discovery_handle_frame_vanish (service, frame);
    }
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

        status = sat_set_create (&object->interests, &(sat_set_args_t)
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

        status = sat_set_create (&object->nodes, &(sat_set_args_t)
                                {
                                    .size = 10,
                                    .object_size = sizeof (sat_discovery_node_t),
                                    .is_equal = sat_discovery_is_equal,
                                    .mode = sat_set_mode_dynamic
                                });
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        strncpy (object->service_name, args->service_name, SAT_DISCOVERY_SERVICE_NAME_MAX_LENGTH);
        strncpy (object->channel.service, args->channel.service, SAT_DISCOVERY_SERVICE_MAX_LENGTH);
        strncpy (object->channel.address, args->channel.address, SAT_DISCOVERY_ADDRESS_MAX_LENGTH);

        sat_uuid_generate_bin (object->uuid);

    } while (false);

    return status;
}

sat_status_t sat_discovery_add_interest (sat_discovery_t *object, const char *service_name)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery add interest error");

    do
    {
        if (object == NULL || service_name == NULL)
        {
            sat_status_set (&status, false, "Invalid parameters");
            break;
        }

        sat_discovery_interest_t interest;
        status = sat_discovery_interest_create (&interest, service_name);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_set_add (object->interests, &interest);

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
        sat_discovery_service_vanish (object);

        status = sat_scheduler_stop (&object->scheduler);

        sleep (1); // Wait a moment to ensure all messages are sent
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
        status = sat_scheduler_open (&object->scheduler, &(sat_scheduler_args_t){.event_amount = 5});
        if (sat_status_get_result (&status) == false)
        {
            break;
        }
        status = sat_scheduler_add_event (&object->scheduler, &(sat_scheduler_event_t)
                                             {
                                                .name = "discovery announce",
                                                .object = object,
                                                .handler = (sat_scheduler_handler_t)sat_discovery_service_announce,
                                                .type = sat_scheduler_type_one_shot,
                                                .timeout = 10
                                             });
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_scheduler_add_event (&object->scheduler, &(sat_scheduler_event_t)
                                             {
                                                .name = "discovery scan",
                                                .object = object,
                                                .handler = (sat_scheduler_handler_t)sat_discovery_service_scan,
                                                .type = sat_scheduler_type_periodic,
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
                                                .handler = (sat_scheduler_handler_t)sat_discovery_service_heartbeat,
                                                .type = sat_scheduler_type_periodic,
                                                .timeout = 5000
                                             });
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_scheduler_add_event (&object->scheduler, &(sat_scheduler_event_t)
                                             {
                                                .name = "discovery interest",
                                                .object = object,
                                                .handler = (sat_scheduler_handler_t)sat_discovery_service_interest,
                                                .type = sat_scheduler_type_periodic,
                                                .timeout = 1000
                                             });

        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_scheduler_add_event (&object->scheduler, &(sat_scheduler_event_t)
                                             {
                                                .name = "discovery ageing",
                                                .object = object,
                                                .handler = (sat_scheduler_handler_t)sat_discovery_service_node_ageing,
                                                .type = sat_scheduler_type_periodic,
                                                .timeout = 1000
                                             });

    } while (false);

    return status;
}

