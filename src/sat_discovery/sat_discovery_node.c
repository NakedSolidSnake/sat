#include <sat_discovery_node.h>
#include <string.h>

sat_status_t sat_discovery_node_create (sat_discovery_node_t *object, sat_discovery_node_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery node create error");

    if (object != NULL && args != NULL)
    {
        strncpy (object->name, args->name, SAT_DISCOVERY_SERVICE_NAME);
        strncpy (object->address, args->address, SAT_DISCOVERY_SERVICE_ADDRESS);
        object->port = args->port;

        sat_status_set (&status, true, "");
    }

    return status;
}