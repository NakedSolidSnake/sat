#ifndef SAT_UUID_H
#define SAT_UUID_H

#include <stdbool.h>


#define SAT_UUID_STRING_SIZE 37
#define SAT_UUID_BINARY_SIZE 16

typedef unsigned char uuid_binary_t [SAT_UUID_BINARY_SIZE];
typedef char uuid_string_t [SAT_UUID_STRING_SIZE];

typedef enum 
{
    sat_uuid_format_lower_case,
    sat_uuid_format_upper_case
} sat_uuid_format_t;

bool sat_uuid_generate_string (uuid_string_t uuid_string, sat_uuid_format_t format);
bool sat_uuid_generate_bin (uuid_binary_t uuid_bin);
bool sat_uuid_string_to_bin (const uuid_string_t uuid_string, uuid_binary_t uuid);
bool sat_uuid_bin_to_string (const uuid_binary_t uuid_bin, uuid_string_t uuid_string, sat_uuid_format_t format);

#endif /* SAT_UUID_H */
