/**
 * @file sat_udp.h
 * @brief UDP communication module for client and server operations
 * @author SAT Library Contributors
 * @date 2025
 * 
 * This module provides a unified interface for UDP communication supporting both
 * client and server modes. It includes unicast and multicast capabilities,
 * synchronous and asynchronous server types, and event-driven programming.
 */

#ifndef SAT_UDP_H_
#define SAT_UDP_H_

#include <sat_status.h>
#include <stdint.h>
#include <sat_udp_types.h>

/**
 * @brief UDP object structure
 * 
 * Main structure representing a UDP endpoint (client or server).
 */
typedef struct 
{
    sat_udp_type_t type;          /**< Type of UDP endpoint (client or server) */
    sat_udp_server_t *server;     /**< Pointer to server instance (if type is server) */
    sat_udp_client_t *client;     /**< Pointer to client instance (if type is client) */

} sat_udp_t;

/**
 * @brief Configuration structure for UDP initialization
 * 
 * Contains all parameters needed to configure either a client or server.
 */
typedef struct 
{
    sat_udp_type_t type;              /**< Type of endpoint to create */
    sat_udp_server_args_t server;     /**< Server configuration (if type is server) */
    sat_udp_client_args_t client;     /**< Client configuration (if type is client) */
    
} sat_udp_args_t;

/**
 * @brief Destination address structure for sending UDP packets
 * 
 * Specifies the target hostname and service (port) for UDP transmission.
 */
typedef struct 
{
    const char *hostname;    /**< Destination hostname or IP address */
    const char *service;     /**< Service name or port number as string */

} sat_udp_destination_t;

/**
 * @brief Initialize a UDP object
 * 
 * Zeros out all fields of the UDP object. Must be called before any other operations.
 * 
 * @param[out] object Pointer to the UDP object to initialize
 * @return sat_status_t indicating success or failure
 * 
 * @note Does not allocate resources. Use sat_udp_open() to configure and allocate
 * @see sat_udp_open()
 */
sat_status_t sat_udp_init (sat_udp_t *const object);

/**
 * @brief Open and configure a UDP endpoint
 * 
 * Creates and configures either a UDP client or server based on the type specified
 * in args. Allocates necessary resources and prepares the endpoint for communication.
 * 
 * @param[in,out] object Pointer to initialized UDP object
 * @param[in] args Pointer to configuration structure
 * @return sat_status_t indicating success or failure
 * 
 * @warning Object must be initialized with sat_udp_init() first
 * @note For servers, this binds to the specified port
 * @note For clients, this prepares the socket for sending
 * @see sat_udp_init(), sat_udp_close()
 */
sat_status_t sat_udp_open (sat_udp_t *const object, const sat_udp_args_t *const args);

/**
 * @brief Run the UDP server (server mode only)
 * 
 * Starts an asynchronous UDP server in a separate thread. Only applicable for
 * server type with async mode.
 * 
 * @param[in,out] object Pointer to opened UDP server object
 * @return sat_status_t indicating success or failure
 * 
 * @warning Only valid for server type with sat_udp_server_type_async
 * @note For interactive servers, use sat_udp_receive() instead
 * @note Server runs in background thread until sat_udp_close() is called
 */
sat_status_t sat_udp_run (sat_udp_t *const object);

/**
 * @brief Send data via UDP
 * 
 * Transmits data to the specified destination. For clients, destination should be
 * provided. For servers, this sends a response to a client.
 * 
 * @param[in] object Pointer to UDP object
 * @param[in] data Pointer to data buffer to send
 * @param[in] size Number of bytes to send
 * @param[in] destination Pointer to destination address (can be NULL for servers)
 * @return sat_status_t indicating success or failure
 * 
 * @note For servers in interactive mode, destination is typically derived from last receive
 * @warning Data size should not exceed maximum UDP packet size (typically 65507 bytes)
 */
sat_status_t sat_udp_send (const sat_udp_t *const object, const char *const data, uint32_t size, const sat_udp_destination_t *const destination);

/**
 * @brief Receive data via UDP
 * 
 * Receives incoming UDP packets. This is a blocking operation that waits for data
 * with an optional timeout.
 * 
 * @param[in] object Pointer to UDP object
 * @param[out] data Pointer to buffer where received data will be stored
 * @param[in,out] size Pointer to size variable (in: buffer size, out: bytes received)
 * @param[in] timeout_ms Timeout in milliseconds (-1 for infinite, 0 for non-blocking)
 * @return sat_status_t indicating success or failure
 * 
 * @warning Buffer must be large enough to hold incoming data
 * @note Only applicable for interactive servers or clients
 * @note Updates size parameter with actual bytes received
 */
sat_status_t sat_udp_receive (const sat_udp_t *const object, char *const data, uint32_t *const size, int timeout_ms);

/**
 * @brief Close the UDP endpoint and free resources
 * 
 * Stops any running threads (for async servers), closes sockets, and deallocates
 * all resources associated with the UDP object.
 * 
 * @param[in,out] object Pointer to UDP object
 * @return sat_status_t indicating success or failure
 * 
 * @note After closing, the object can be reused by calling sat_udp_init() and sat_udp_open()
 * @warning Any pending data may be lost
 */
sat_status_t sat_udp_close (sat_udp_t *const object);

/**
 * @brief Get the port number of the UDP endpoint
 * 
 * Retrieves the port number that the UDP endpoint is bound to or using.
 * 
 * @param[in] object Pointer to UDP object
 * @param[out] port Pointer to variable where port number will be stored
 * @return sat_status_t indicating success or failure
 * 
 * @note Useful for servers to discover which port was assigned
 * @note For clients, returns the local port used for communication
 */
sat_status_t sat_udp_get_port (const sat_udp_t *const object, uint16_t *const port);

#endif/* SAT_UDP_H_ */

