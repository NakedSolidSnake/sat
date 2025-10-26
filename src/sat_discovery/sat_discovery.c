#include <sat_discovery.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

static sat_status_t sat_discovery_is_args_valid (sat_discovery_args_t *args);
static sat_status_t sat_discovery_server_setup (sat_discovery_t *object, sat_discovery_args_t *args);

static void *sat_discovery_run (void *arg);

static void sat_discovery_on_receive (char *buffer, uint32_t *size, void *data)
{
    (void)buffer;
    (void)size;
    sat_discovery_t *service = (sat_discovery_t *)data;
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

        strncpy (object->service_name, args->name, SAT_DISCOVERY_SERVICE_NAME_MAX_LENGTH);
        strncpy (object->channel.service, args->channel.service, SAT_DISCOVERY_SERVICE_MAX_LENGTH);
        strncpy (object->channel.address, args->channel.address, SAT_DISCOVERY_ADDRESS_MAX_LENGTH);

    } while (false);

    return status;
}

sat_status_t sat_discovery_start (sat_discovery_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery start error");

    if (object != NULL )
    {
        pthread_t thread;

        object->is_running = true;

        pthread_create (&thread, NULL, sat_discovery_run, (void *)object);
        pthread_detach (thread);


        // Start the discovery process
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_discovery_stop (sat_discovery_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery stop error");

    if (object != NULL )
    {
        // Stop the discovery process
        object->is_running = false;
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_discovery_announce (sat_discovery_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery announce error");

    if (object != NULL )
    {
        sat_udp_send (&object->udp, object->service_name, strlen (object->service_name), &(sat_udp_destination_t)
                                                                          {
                                                                            .hostname = object->channel.address,
                                                                            .service = object->channel.service
                                                                          });
        
        sat_status_set (&status, true, "");
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

static void *sat_discovery_run (void *arg)
{
    sat_discovery_t *object = (sat_discovery_t *)arg;

    if (object != NULL)
    {
        // Discovery main loop
        while (object->is_running)
        {
            sat_discovery_announce (object);
            sat_udp_run (&object->udp);
        }
    }

    return NULL;
}