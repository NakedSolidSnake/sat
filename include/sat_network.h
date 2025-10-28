#ifndef SAT_NETWORK_H
#define SAT_NETWORK_H

#include <sat_status.h>
#include <stdint.h>

#define SAT_NETWORK_IP_MAX_LEN 46

sat_status_t sat_network_get_public_ip (const char *const dns_address, uint16_t dns_port, char ip_address [SAT_NETWORK_IP_MAX_LEN + 1]);

#endif /* SAT_NETWORK_H */
