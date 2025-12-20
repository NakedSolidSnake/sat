/**
 * @file sat_discovery.h
 * @brief Distributed service discovery system with automatic registration
 * 
 * This module provides a zero-configuration service discovery protocol that
 * allows services to automatically announce themselves and discover other
 * services on the network. It uses UDP multicast for communication and
 * implements heartbeat monitoring, interest-based filtering, and automatic
 * node aging.
 * 
 * The discovery system is fully distributed with no central server, making
 * it resilient and scalable. Services automatically announce their presence,
 * maintain connectivity through heartbeats, and gracefully handle departures.
 */

#ifndef SAT_DISCOVERY_H
#define SAT_DISCOVERY_H

#include <sat_status.h>
#include <sat_udp.h>
#include <sat_scheduler.h>
#include <sat_set.h>
#include <sat_array.h>
#include <sat_uuid.h>

/** @brief Maximum length for service names */
#define SAT_DISCOVERY_SERVICE_NAME_MAX_LENGTH 128

/** @brief Maximum length for IP addresses */
#define SAT_DISCOVERY_ADDRESS_MAX_LENGTH 64

/** @brief Maximum length for service port strings */
#define SAT_DISCOVERY_SERVICE_MAX_LENGTH 6

/** @brief Maximum length for network interface names */
#define SAT_DISCOVERY_INTERFACE_NAME_SIZE  64

/** @brief Maximum length for application port strings */
#define SAT_DISCOVERY_APP_PORT_SIZE 6

/**
 * @brief Service information structure
 * 
 * Contains the essential information about a discovered service,
 * including its name, network address, and port.
 */
typedef struct
{
    char name [SAT_DISCOVERY_SERVICE_NAME_MAX_LENGTH + 1];    /**< Service name */
    char address [SAT_DISCOVERY_ADDRESS_MAX_LENGTH + 1];      /**< Service IP address */
    char port [SAT_DISCOVERY_APP_PORT_SIZE + 1];              /**< Service port */
} sat_discovery_service_info_t;

/**
 * @brief Discovery service instance structure
 * 
 * Represents a discovery service that can announce itself and discover
 * other services on the network. Contains all internal state including
 * UDP communication, scheduler, interest list, and discovered nodes.
 */
typedef struct
{
    sat_udp_t udp;                  /**< UDP server for multicast communication */
    sat_scheduler_t scheduler;       /**< Scheduler for periodic tasks */
    sat_set_t *interests;            /**< Set of service names we're interested in */
    sat_set_t *nodes;                /**< Set of discovered nodes */
    sat_uuid_binary_t uuid;          /**< Unique identifier for this instance */

    sat_array_t *interfaces;         /**< Network interfaces information */
    
    char service_name [SAT_DISCOVERY_SERVICE_NAME_MAX_LENGTH + 1];  /**< This service's name */

    struct
    {
        char service [SAT_DISCOVERY_SERVICE_MAX_LENGTH + 1];       /**< Multicast port */
        char address [SAT_DISCOVERY_ADDRESS_MAX_LENGTH + 1];       /**< Multicast address */
    } channel;  /**< Multicast channel configuration */

    char app_port [SAT_DISCOVERY_APP_PORT_SIZE + 1];  /**< Application service port */
    
} sat_discovery_t;

/**
 * @brief Discovery configuration structure
 * 
 * Configuration parameters for initializing a discovery service.
 */
typedef struct
{
    struct 
    {
        char *name;         /**< Name of this service */
        char *interface;    /**< Network interface to use (NULL for all) */
        char *port;         /**< Port this service listens on */
    } service;  /**< Service-specific configuration */

    struct 
    {
        char *service;      /**< Multicast port for discovery */
        char *address;      /**< Multicast address for discovery */
    } channel;  /**< Multicast channel configuration */

} sat_discovery_args_t;

/**
 * @brief Initialize a discovery service object
 * 
 * Initializes a discovery service instance by clearing all fields.
 * Must be called before sat_discovery_open().
 * 
 * @param object Pointer to discovery service object to initialize
 * @return Status indicating success or failure
 */
sat_status_t sat_discovery_init (sat_discovery_t *object);

/**
 * @brief Open and configure a discovery service
 * 
 * Configures the discovery service with the specified parameters, sets up
 * the UDP multicast server, initializes the scheduler with periodic tasks,
 * and prepares the service for operation.
 * 
 * The service will be ready to announce itself and discover others, but
 * will not start until sat_discovery_start() is called.
 * 
 * @param object Pointer to initialized discovery object
 * @param args Pointer to configuration structure
 * @return Status indicating success or failure
 */
sat_status_t sat_discovery_open (sat_discovery_t *object, sat_discovery_args_t *args);

/**
 * @brief Register interest in a specific service
 * 
 * Adds a service name to the interest list. When this service announces
 * itself, it will specifically request information about interested services,
 * and will track their availability.
 * 
 * Multiple interests can be registered by calling this function multiple times.
 * 
 * @param object Pointer to discovery service object
 * @param service_name Name of the service to watch for
 * @return Status indicating success or failure
 */
sat_status_t sat_discovery_add_interest (sat_discovery_t *object, const char *service_name);

/**
 * @brief Start the discovery service
 * 
 * Starts the discovery service by activating the scheduler. The service will:
 * - Announce itself on the network
 * - Send periodic heartbeats
 * - Listen for other service announcements
 * - Send interest queries for registered services
 * - Maintain the list of active nodes
 * 
 * @param object Pointer to discovery service object
 * @return Status indicating success or failure
 */
sat_status_t sat_discovery_start (sat_discovery_t *object);

/**
 * @brief Stop the discovery service
 * 
 * Stops the discovery service by sending a vanish message to inform other
 * nodes of departure, then stopping the scheduler. The service can be
 * restarted with sat_discovery_start().
 * 
 * @param object Pointer to discovery service object
 * @return Status indicating success or failure
 */
sat_status_t sat_discovery_stop (sat_discovery_t *object);

/**
 * @brief Retrieve information about a discovered service
 * 
 * Queries the list of discovered nodes for a specific service by name
 * and retrieves its network information (address and port).
 * 
 * @param object Pointer to discovery service object
 * @param service Name of the service to query
 * @param info Pointer to structure to receive service information
 * @return Status indicating success if found, failure if not found
 */
sat_status_t sat_discovery_get_service_info (sat_discovery_t *object, const char *const service, sat_discovery_service_info_t *const info);

/**
 * @brief Print all registered services
 * 
 * Logs information about all currently discovered and active services
 * using the sat_log system. Useful for debugging and monitoring.
 * 
 * @param object Pointer to discovery service object
 */
void sat_discovery_registered_services (sat_discovery_t *object);

#endif /* SAT_DISCOVERY_H */
