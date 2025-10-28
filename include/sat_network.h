#ifndef SAT_NETWORK_H
#define SAT_NETWORK_H

#include <sat_status.h>
#include <stdint.h>

typedef enum
{
    sat_network_class_multicast,
    sat_network_class_broadcast,
    sat_network_class_public,
    sat_network_class_private,
    sat_network_class_loopback,

} sat_network_class_t;

#define SAT_NETWORK_IP_MAX_LEN 46

sat_status_t sat_network_get_ip_class (const char *const ip_address, sat_network_class_t *const ip_class);
const char *const sat_network_get_ip_class_string (const sat_network_class_t ip_class);

sat_status_t sat_network_get_public_ip (const char *const dns_address, uint16_t dns_port, char ip_address [SAT_NETWORK_IP_MAX_LEN + 1]);

#endif /* SAT_NETWORK_H */
