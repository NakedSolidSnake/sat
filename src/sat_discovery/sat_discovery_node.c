#include <sat_discovery_node.h>
#include <string.h>
#include <sat_time.h>
#include <sat_uuid.h>
#include <stdio.h>

void sat_discovery_node_debug (const sat_discovery_node_t *const object)
{
    if (object != NULL)
    {
        char uuid_string [37] = {0};

        sat_uuid_bin_to_string (object->uuid, uuid_string, sat_uuid_format_upper_case);

        printf ("Discovery Node UUID: %s", uuid_string);
        printf ("  Name: %s", object->name);
        printf ("  Address: %s", object->address);
        printf ("  Port: %u", object->port);
        printf ("  Last Heartbeat: %u\n", object->last_heartbeat);
    }
}

sat_status_t sat_discovery_node_create (sat_discovery_node_t *object, sat_discovery_node_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery node create error");

    if (object != NULL && args != NULL)
    {
        memset (object, 0, sizeof (sat_discovery_node_t));

        memcpy (object->uuid, args->uuid, sizeof (uuid_binary_t));

        strncpy (object->name, args->name, SAT_DISCOVERY_SERVICE_NAME);
        strncpy (object->address, args->address, SAT_DISCOVERY_SERVICE_ADDRESS);
        object->port = args->port;

        object->last_heartbeat = sat_time_get_utc_epoch_now ();

        sat_discovery_node_debug (object);

        sat_status_success (&status);
    }

    return status;
}

sat_status_t sat_discovery_node_update_last_seen (sat_discovery_node_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery node update last seen error");

    if (object != NULL)
    {
        object->last_heartbeat = sat_time_get_utc_epoch_now ();

        sat_discovery_node_debug (object);

        sat_status_success (&status);
    }

    return status;
}