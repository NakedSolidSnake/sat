#include <sat_uuid.h>
#include <uuid/uuid.h>
#include <string.h>

bool sat_uuid_generate_string (uuid_string_t uuid_string, sat_uuid_format_t format)
{
    uuid_t binuuid;

    uuid_generate_random (binuuid);

    memset (uuid_string, 0, SAT_UUID_STRING_SIZE);

    switch (format)
    {
    case sat_uuid_format_upper_case:
        uuid_unparse_upper (binuuid, uuid_string);
        break;

    case sat_uuid_format_lower_case:
        uuid_unparse_lower (binuuid, uuid_string);
        break;

    default:
        uuid_unparse (binuuid, uuid_string);
        break;
    }

    return true;
}

bool sat_uuid_generate_bin (uuid_binary_t uuid_bin)
{
    uuid_t binuuid;

    uuid_generate_random (binuuid);
    memcpy (uuid_bin, binuuid, SAT_UUID_BINARY_SIZE);

    return true;
}

bool sat_uuid_string_to_bin (const uuid_string_t uuid_string, uuid_binary_t uuid)
{
    uuid_parse ((const char *)uuid_string, uuid);

    return true;
}

bool sat_uuid_bin_to_string (const uuid_binary_t uuid_bin, uuid_string_t uuid_string, sat_uuid_format_t format)
{
    memset (uuid_string, 0, SAT_UUID_STRING_SIZE);

    switch (format)
    {
    case sat_uuid_format_upper_case:
        uuid_unparse_upper (uuid_bin, uuid_string);
        break;

    case sat_uuid_format_lower_case:
        uuid_unparse_lower (uuid_bin, uuid_string);
        break;

    default:
        uuid_unparse (uuid_bin, uuid_string);
        break;
    }

    return true;
}