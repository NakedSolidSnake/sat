#ifndef SAT_DISCOVERY_H
#define SAT_DISCOVERY_H

#include <sat_status.h>
#include <sat_udp.h>
#include <sat_scheduler.h>
#include <sat_set.h>
#include <sat_uuid.h>

#define SAT_DISCOVERY_SERVICE_NAME_MAX_LENGTH 128
#define SAT_DISCOVERY_ADDRESS_MAX_LENGTH 64
#define SAT_DISCOVERY_SERVICE_MAX_LENGTH 6

typedef struct
{
    sat_udp_t udp;
    sat_scheduler_t scheduler;
    sat_set_t *interests;
    sat_set_t *nodes;
    uuid_binary_t uuid;
    
    char service_name [SAT_DISCOVERY_SERVICE_NAME_MAX_LENGTH + 1];

    struct
    {
        char service [SAT_DISCOVERY_SERVICE_MAX_LENGTH + 1];
        char address [SAT_DISCOVERY_ADDRESS_MAX_LENGTH + 1];
    } channel;
    
} sat_discovery_t;

typedef struct
{
    char *service_name;
    struct 
    {
        char *service;
        char *address;
    } channel;

} sat_discovery_args_t;

sat_status_t sat_discovery_init (sat_discovery_t *object);
sat_status_t sat_discovery_open (sat_discovery_t *object, sat_discovery_args_t *args);

sat_status_t sat_discovery_add_interest (sat_discovery_t *object, const char *service_name);

sat_status_t sat_discovery_start (sat_discovery_t *object);
sat_status_t sat_discovery_stop (sat_discovery_t *object);

#endif /* SAT_DISCOVERY_H */
