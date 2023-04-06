#ifndef SAT_UUID_H
#define SAT_UUID_H

#include <stdbool.h>

typedef enum 
{
    sat_uuid_format_lower_case,
    sat_uuid_format_upper_case
} sat_uuid_format_t;

bool sat_uuid_generate (char uuid[37], sat_uuid_format_t format);

#endif/* SAT_UUID_H */
