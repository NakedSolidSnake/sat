#include <sat_discovery_node.h>
#include <string.h>
#include <sat_time.h>
#include <sat_uuid.h>
#include <stdio.h>


sat_status_t sat_discovery_node_create (sat_discovery_node_t *object, sat_discovery_node_args_t *args)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (args, "null args");

    memset (object, 0, sizeof (sat_discovery_node_t));

    memcpy (object->uuid, args->uuid, sizeof (sat_uuid_binary_t));

    strncpy (object->name, args->name, SAT_DISCOVERY_SERVICE_NAME);
    strncpy (object->address, args->address, SAT_DISCOVERY_SERVICE_ADDRESS);
    strncpy (object->port, args->port, SAT_DISCOVERY_SERVICE_PORT);

    object->last_heartbeat = sat_time_get_utc_epoch_now ();

    sat_status_return_on_success ();
}

sat_status_t sat_discovery_node_update_last_seen (sat_discovery_node_t *object)
{
    sat_status_return_on_null (object, "null object");

    object->last_heartbeat = sat_time_get_utc_epoch_now ();

    sat_status_return_on_success ();
}