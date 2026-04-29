#include <sat_discovery_interest.h>
#include <string.h>

sat_status_t sat_discovery_interest_create (sat_discovery_interest_t *object, const sat_discovery_interest_args_t *const args)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (args, "null args");

    memset (object->name, 0, sizeof (sat_discovery_interest_t));

    strncpy (object->name, args->service_name, SAT_DISCOVERY_INTEREST_NAME_SIZE);

    if (args->uuid != NULL)
    {
        memcpy (object->uuid, args->uuid, sizeof (sat_uuid_binary_t));
    }

    sat_status_return_on_success ();
}

sat_status_t sat_discovery_interest_clear (sat_discovery_interest_t *object)
{
    sat_status_return_on_null (object, "null object");

    memset (object->uuid, 0, sizeof (sat_uuid_binary_t));
    object->registered = false;

    sat_status_return_on_success ();
}