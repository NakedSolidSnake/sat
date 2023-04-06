#include <sat_uuid.h>
#include <uuid/uuid.h>

bool sat_uuid_generate(char uuid[37], sat_uuid_format_t format)
{
    uuid_t binuuid;

    uuid_generate_random(binuuid);

    switch (format)
    {
    case sat_uuid_format_upper_case:
        uuid_unparse_upper(binuuid, uuid);
        break;

    case sat_uuid_format_lower_case:
        uuid_unparse_lower(binuuid, uuid);
        break;

    default:
        uuid_unparse(binuuid, uuid);
        break;
    }

    return true;
}