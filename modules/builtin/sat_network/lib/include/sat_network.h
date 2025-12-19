/**
 * @file sat_network.h
 * @brief Network utility functions for network interface information and IP classification
 * @author SAT Library Team
 * @date December 2025
 * 
 * This module provides utilities for working with network interfaces, IP addresses,
 * and network information. It supports both IPv4 and IPv6 addresses and can retrieve
 * detailed information about network interfaces on the system.
 */

#ifndef SAT_NETWORK_H
#define SAT_NETWORK_H

#include <sat_status.h>
#include <stdint.h>
#include <sat_array.h>

/** Maximum size for network address string */
#define SAT_NETWORK_ADDRESS_SIZE        50

/** Maximum size for network interface name */
#define SAT_NETWORK_INTERFACE_NAME_SIZE 32

/** Maximum length for IP address string (supports IPv6) */
#define SAT_NETWORK_IP_MAX_LEN          46

/**
 * @brief Network address family types
 */
typedef enum
{
    sat_network_family_ipv4,     /**< IPv4 address family */
    sat_network_family_ipv6,     /**< IPv6 address family */
    sat_network_family_unknown   /**< Unknown address family */

} sat_network_family_t;

/**
 * @brief IP address classification types
 */
typedef enum
{
    sat_network_class_multicast, /**< Multicast address (224.0.0.0 - 239.255.255.255) */
    sat_network_class_broadcast, /**< Broadcast address (255.255.255.255) */
    sat_network_class_public,    /**< Public routable address */
    sat_network_class_private,   /**< Private address (10.x, 172.16-31.x, 192.168.x) */
    sat_network_class_loopback,  /**< Loopback address (127.x.x.x) */

} sat_network_class_t;

/**
 * @brief Socket information structure
 */
typedef struct
{
    char address [SAT_NETWORK_ADDRESS_SIZE + 1]; /**< IP address string */
    uint16_t port;                                /**< Port number */
} sat_network_socket_info_t;

/**
 * @brief Network interface information structure
 */
typedef struct 
{
    char interface_name[SAT_NETWORK_INTERFACE_NAME_SIZE + 1]; /**< Interface name (e.g., eth0, wlan0) */
    char ip_address [SAT_NETWORK_ADDRESS_SIZE + 1];           /**< IP address of the interface */
    char netmask [SAT_NETWORK_ADDRESS_SIZE + 1];              /**< Network mask */
    sat_network_family_t family;                               /**< Address family (IPv4/IPv6) */
} sat_network_info_t;

/**
 * @brief Get the classification of an IP address
 * 
 * Classifies an IP address into one of the defined categories: multicast, broadcast,
 * public, private, or loopback.
 * 
 * @param[in] ip_address IP address string to classify
 * @param[out] ip_class Pointer to store the IP address class
 * @return Status indicating success or failure
 */
sat_status_t sat_network_get_ip_class (const char *const ip_address, sat_network_class_t *const ip_class);

/**
 * @brief Get string representation of IP class
 * 
 * Converts an IP class enumeration value to its string representation.
 * 
 * @param[in] ip_class IP class enumeration value
 * @return String representation of the IP class
 */
const char *const sat_network_get_ip_class_string (const sat_network_class_t ip_class);

/**
 * @brief Get the public IP address of this machine
 * 
 * Retrieves the public IP address by querying a DNS server. Useful for determining
 * the external-facing IP address of a machine behind NAT.
 * 
 * @param[in] dns_address DNS server address to query (NULL for default: 8.8.8.8)
 * @param[in] dns_port DNS server port (0 for default: 53)
 * @param[out] ip_address Buffer to store the public IP address
 * @return Status indicating success or failure
 */
sat_status_t sat_network_get_public_ip (const char *const dns_address, uint16_t dns_port, char ip_address [SAT_NETWORK_IP_MAX_LEN + 1]);

/**
 * @brief Get socket information from a socket file descriptor
 * 
 * Retrieves the address and port information associated with a socket.
 * 
 * @param[in] socket Socket file descriptor
 * @param[out] socket_info Pointer to structure to receive socket information
 * @return Status indicating success or failure
 */
sat_status_t sat_network_get_socket_info (int socket, sat_network_socket_info_t *const socket_info);

/**
 * @brief Get network information for a specific interface
 * 
 * Retrieves detailed information about a network interface by name.
 * 
 * @param[out] info Pointer to structure to receive network information
 * @param[in] interface_name Name of the network interface (e.g., "eth0", "wlan0")
 * @return Status indicating success or failure
 */
sat_status_t sat_network_get_info_by_interface (sat_network_info_t *const info, const char *const interface_name);

/**
 * @brief Get list of all network interfaces
 * 
 * Retrieves information about all network interfaces on the system. The returned
 * array contains sat_network_info_t structures.
 * 
 * @param[out] info Pointer to receive the array of network interfaces
 * @return Status indicating success or failure
 */
sat_status_t sat_network_get_info_list (sat_array_t **const info);

/**
 * @brief Print socket information to stdout for debugging
 * 
 * Outputs formatted socket information including address and port.
 * 
 * @param[in] socket_info Pointer to socket information structure
 */
void sat_network_socket_info_debug (const sat_network_socket_info_t *const socket_info);

/**
 * @brief Print network interface information to stdout for debugging
 * 
 * Outputs formatted network interface information including name, IP, netmask, and family.
 * 
 * @param[in] network_info Pointer to network information structure
 */
void sat_network_info_debug (const sat_network_info_t *const network_info);

/**
 * @brief Print list of network interfaces to stdout for debugging
 * 
 * Outputs formatted information for all network interfaces in the provided array.
 * 
 * @param[in] info Array of network information structures
 */
void sat_network_info_list_debug (const sat_array_t *const info);

#endif /* SAT_NETWORK_H */
