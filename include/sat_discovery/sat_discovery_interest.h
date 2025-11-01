#ifndef SAT_DISCOVERY_INTEREST_H
#define SAT_DISCOVERY_INTEREST_H

#include <sat_status.h>
#include <stdint.h>

#define SAT_DISCOVERY_INTEREST_NAME_SIZE 31

typedef struct 
{
    char name [SAT_DISCOVERY_INTEREST_NAME_SIZE + 1];
    bool registered;
    
} sat_discovery_interest_t;

sat_status_t sat_discovery_interest_create (sat_discovery_interest_t *object, const char *const service_name);


#endif /* SAT_DISCOVERY_INTEREST_H */
