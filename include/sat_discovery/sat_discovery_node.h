#ifndef SAT_DISCOVERY_NODE_H
#define SAT_DISCOVERY_NODE_H

#include <stdint.h>
#include <sat_status.h>
#include <sat_uuid.h>

#define SAT_DISCOVERY_SERVICE_NAME         31
#define SAT_DISCOVERY_SERVICE_ADDRESS      64
#define SAT_DISCOVERY_SERVICE_PORT         6

typedef struct
{
    sat_uuid_binary_t uuid;
    char name [SAT_DISCOVERY_SERVICE_NAME + 1];
    char address [SAT_DISCOVERY_SERVICE_ADDRESS + 1];
    char port [SAT_DISCOVERY_SERVICE_PORT + 1];
    uint32_t last_heartbeat;

} sat_discovery_node_t;

typedef struct
{
    sat_uuid_binary_t *uuid;
    char *name;
    char *address;
    char *port;
} sat_discovery_node_args_t;

sat_status_t sat_discovery_node_create (sat_discovery_node_t *object, sat_discovery_node_args_t *args);
sat_status_t sat_discovery_node_update_last_seen (sat_discovery_node_t *object);

#endif /* SAT_DISCOVERY_NODE_H */
