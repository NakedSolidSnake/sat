#ifndef SAT_NETWORK_H
#define SAT_NETWORK_H

#include <sat_status.h>
#include <stdint.h>
#include <sat_array.h>

#define SAT_NETWORK_ADDRESS_SIZE        50
#define SAT_NETWORK_INTERFACE_NAME_SIZE 32
#define SAT_NETWORK_IP_MAX_LEN          46

typedef enum
{
    sat_network_family_ipv4,
    sat_network_family_ipv6,
    sat_network_family_unknown

} sat_network_family_t;

typedef enum
{
    sat_network_class_multicast,
    sat_network_class_broadcast,
    sat_network_class_public,
    sat_network_class_private,
    sat_network_class_loopback,

} sat_network_class_t;

typedef struct
{
    char address [SAT_NETWORK_ADDRESS_SIZE + 1];
    uint16_t port;
} sat_network_socket_info_t;

typedef struct 
{
    char interface_name[SAT_NETWORK_INTERFACE_NAME_SIZE + 1];
    char ip_address [SAT_NETWORK_ADDRESS_SIZE + 1];
    char netmask [SAT_NETWORK_ADDRESS_SIZE + 1];
    sat_network_family_t family;
} sat_network_info_t;

sat_status_t sat_network_get_ip_class (const char *const ip_address, sat_network_class_t *const ip_class);
const char *const sat_network_get_ip_class_string (const sat_network_class_t ip_class);

sat_status_t sat_network_get_public_ip (const char *const dns_address, uint16_t dns_port, char ip_address [SAT_NETWORK_IP_MAX_LEN + 1]);

sat_status_t sat_network_get_socket_info (int socket, sat_network_socket_info_t *const socket_info);
sat_status_t sat_network_get_info_by_interface (sat_network_info_t *const info, const char *const interface_name);
sat_status_t sat_network_get_info_list (sat_array_t **info);
void sat_network_socket_info_debug (const sat_network_socket_info_t *const socket_info);
void sat_network_info_debug (const sat_network_info_t *const network_info);
void sat_network_info_list_debug (sat_array_t *info);

#endif /* SAT_NETWORK_H */
