#ifndef SAT_DISCOVERY_NODE_H
#define SAT_DISCOVERY_NODE_H

#include <stdint.h>
#include <sat_status.h>

#define SAT_DISCOVERY_SERVICE_NAME         31
#define SAT_DISCOVERY_SERVICE_ADDRESS      64

typedef struct
{
    char name [SAT_DISCOVERY_SERVICE_NAME + 1];
    char address [SAT_DISCOVERY_SERVICE_ADDRESS + 1];
    uint16_t port;
    uint32_t last_heartbeat;

} sat_discovery_node_t;

typedef struct
{
    char *name;
    char *address;
    uint16_t port;
} sat_discovery_node_args_t;

sat_status_t sat_discovery_node_create (sat_discovery_node_t *object, sat_discovery_node_args_t *args);

#endif /* SAT_DISCOVERY_NODE_H */
