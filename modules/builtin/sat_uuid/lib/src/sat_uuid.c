#include <sat_uuid.h>
#include <uuid/uuid.h>
#include <string.h>

bool sat_uuid_generate_string (sat_uuid_string_t uuid_string, sat_uuid_format_t format)
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

bool sat_uuid_generate_bin (sat_uuid_binary_t uuid_bin)
{
    uuid_t binuuid;

    uuid_generate_random (binuuid);
    memcpy (uuid_bin, binuuid, SAT_UUID_BINARY_SIZE);

    return true;
}

bool sat_uuid_string_to_bin (const sat_uuid_string_t uuid_string, sat_uuid_binary_t uuid)
{
    uuid_parse ((const char *)uuid_string, uuid);

    return true;
}

bool sat_uuid_bin_to_string (const sat_uuid_binary_t uuid_bin, sat_uuid_string_t uuid_string, sat_uuid_format_t format)
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

bool sat_uuid_compare_bin (const sat_uuid_binary_t uuid1, const sat_uuid_binary_t uuid2)
{
    return uuid_compare (uuid1, uuid2) == 0;
}

bool sat_uuid_compare_string (const sat_uuid_string_t uuid1, const sat_uuid_string_t uuid2, sat_uuid_format_t format)
{
    bool status = true;

    for (int i = 0; i < SAT_UUID_STRING_SIZE; i++)
    {
        if (uuid1 [i] != uuid2 [i])
        {
            status = false;
            break;
        }
    }

    return status;
}
