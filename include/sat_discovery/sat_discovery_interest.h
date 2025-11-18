#ifndef SAT_DISCOVERY_INTEREST_H
#define SAT_DISCOVERY_INTEREST_H

#include <sat_status.h>
#include <stdint.h>
#include <sat_uuid.h>

#define SAT_DISCOVERY_INTEREST_NAME_SIZE 31

typedef struct 
{
    char name [SAT_DISCOVERY_INTEREST_NAME_SIZE + 1];
    sat_uuid_binary_t uuid;
    bool registered;
    
} sat_discovery_interest_t;

typedef struct 
{
    const char *const service_name;
    sat_uuid_binary_t *const uuid;

} sat_discovery_interest_args_t;

sat_status_t sat_discovery_interest_create (sat_discovery_interest_t *object, const sat_discovery_interest_args_t *const args);


#endif /* SAT_DISCOVERY_INTEREST_H */
