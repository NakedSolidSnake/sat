/**
 * @file sat_udp_types.h
 * @brief Type definitions and structures for UDP module
 * @author SAT Library Contributors
 * @date 2025
 * 
 * This file contains all type definitions, enumerations, and callback function
 * prototypes used by the SAT UDP module.
 */

#ifndef SAT_UDP_TYPES_H_
#define SAT_UDP_TYPES_H_

#include <stdint.h>

/**
 * @brief Maximum hostname buffer size
 */
#define SAT_UDP_HOSTNAME_SIZE       1024

/**
 * @brief Callback function type for UDP data events
 * 
 * Invoked when data is received or before data is sent.
 * 
 * @param buffer Pointer to data buffer
 * @param size Pointer to size variable (in: buffer size, out: processed size)
 * @param data User-defined context pointer
 */
typedef void (*sat_udp_event_t) (char *const buffer, uint32_t *const size, void *const data);

/**
 * @brief Callback function type for multicast events
 * 
 * Invoked when joining or leaving a multicast group.
 * 
 * @param data User-defined context pointer
 */
typedef void (*sat_udp_multicast_event_t) (void *const data);

/**
 * @brief UDP endpoint type enumeration
 */
typedef enum 
{
    sat_udp_type_server,    /**< UDP server mode */
    sat_udp_type_client     /**< UDP client mode */

} sat_udp_type_t;

/**
 * @brief UDP server operation type
 */
typedef enum 
{
    sat_udp_server_type_interactive,    /**< Synchronous server - manual receive calls */
    sat_udp_server_type_async,          /**< Asynchronous server - automatic event-driven */

} sat_udp_server_type_t;

/**
 * @brief UDP server communication mode
 */
typedef enum 
{
    sat_udp_server_mode_unicast,        /**< Standard point-to-point communication */
    sat_udp_server_mode_multicast,      /**< Multicast group communication */

} sat_udp_server_mode_t;

/**
 * @brief Opaque server structure
 * 
 * Internal implementation details are hidden from the user.
 */
typedef struct sat_udp_server_t sat_udp_server_t;

/**
 * @brief Opaque client structure
 * 
 * Internal implementation details are hidden from the user.
 */
typedef struct sat_udp_client_t sat_udp_client_t;

/**
 * @brief Server configuration structure
 * 
 * Contains all parameters needed to configure a UDP server.
 */
typedef struct 
{
    const char *multicast_group;    /**< Multicast group IP (e.g., "239.0.0.1") for multicast mode */
    const char *service;             /**< Service name or port number as string */
    char *buffer;                    /**< Receive buffer for incoming data */
    uint32_t size;                   /**< Size of receive buffer in bytes */

    /**
     * @brief Event callbacks for asynchronous operations
     */
    struct 
    {
        sat_udp_event_t on_receive;                 /**< Called when data is received */
        sat_udp_event_t on_send;                    /**< Called before data is sent */
        sat_udp_multicast_event_t on_multicast_join; /**< Called when joining multicast group */
        
    } events;

    void *data;                      /**< User-defined context pointer passed to callbacks */

    sat_udp_server_type_t type;      /**< Server operation type (interactive or async) */

    sat_udp_server_mode_t mode;      /**< Communication mode (unicast or multicast) */

} sat_udp_server_args_t;

/**
 * @brief Client configuration structure
 * 
 * Contains parameters needed to configure a UDP client.
 */
typedef struct 
{
    char *hostname;        /**< Hostname or IP address buffer (must be writable) */
    const char *service;   /**< Service name or port number as string */

} sat_udp_client_args_t;

#endif/* SAT_UDP_TYPES_H_ */
