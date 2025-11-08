#ifndef SAT_UUID_H
#define SAT_UUID_H

#include <stdbool.h>


#define SAT_UUID_STRING_SIZE 37
#define SAT_UUID_BINARY_SIZE 16

typedef unsigned char sat_uuid_binary_t [SAT_UUID_BINARY_SIZE];
typedef char sat_uuid_string_t [SAT_UUID_STRING_SIZE];

typedef enum 
{
    sat_uuid_format_lower_case,
    sat_uuid_format_upper_case
} sat_uuid_format_t;

bool sat_uuid_generate_string (sat_uuid_string_t uuid_string, sat_uuid_format_t format);
bool sat_uuid_generate_bin (sat_uuid_binary_t uuid_bin);
bool sat_uuid_string_to_bin (const sat_uuid_string_t uuid_string, sat_uuid_binary_t uuid);
bool sat_uuid_bin_to_string (const sat_uuid_binary_t uuid_bin, sat_uuid_string_t uuid_string, sat_uuid_format_t format);
bool sat_uuid_compare_bin (const sat_uuid_binary_t uuid1, const sat_uuid_binary_t uuid2);
bool sat_uuid_compare_string (const sat_uuid_string_t uuid1, const sat_uuid_string_t uuid2, sat_uuid_format_t format);

#endif /* SAT_UUID_H */
