#include <sat_discovery_node.h>
#include <string.h>
#include <sat_time.h>

sat_status_t sat_discovery_node_create (sat_discovery_node_t *object, sat_discovery_node_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery node create error");

    if (object != NULL && args != NULL)
    {
        strncpy (object->name, args->name, SAT_DISCOVERY_SERVICE_NAME);
        strncpy (object->address, args->address, SAT_DISCOVERY_SERVICE_ADDRESS);
        object->port = args->port;

        object->last_heartbeat = sat_time_get_utc_epoch_now ();

        sat_status_success (&status);
    }

    return status;
}