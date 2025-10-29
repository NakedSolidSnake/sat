#include <sat_discovery_interest.h>
#include <string.h>

sat_status_t sat_discovery_interest_create (sat_discovery_interest_t *object, const char *const service_name)
{
    sat_status_t status = sat_status_set (&status, false, "sat discovery interest create error");

    if (object != NULL && service_name != NULL)
    {
        memset (object->name, 0, sizeof (sat_discovery_interest_t));

        strncpy (object->name, service_name, SAT_DISCOVERY_INTEREST_NAME_SIZE);

        sat_status_set (&status, true, "");
    }

    return status;
}