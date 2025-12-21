/**
 * @file sat_queue_ipc.h
 * @brief System V message queue IPC implementation
 * 
 * This module provides a high-level interface to System V message queues for
 * inter-process communication (IPC). It wraps the POSIX message queue functions
 * (msgget, msgsnd, msgrcv, msgctl) with a convenient and safe API.
 * 
 * System V message queues allow processes to exchange data in a structured
 * format with type-based message filtering. Messages persist in the kernel
 * until explicitly destroyed or system reboot.
 */

#ifndef SAT_QUEUE_IPC_H_
#define SAT_QUEUE_IPC_H_

#include <sat_status.h>
#include <stdlib.h>

/**
 * @brief Message queue permission flags
 * 
 * Defines access permissions for the message queue using octal notation.
 */
typedef enum 
{
    sat_queue_ipc_flags_read_and_write = 0666,  /**< Read and write for all users */

} sat_queue_ipc_flags_t;

/**
 * @brief Message queue instance structure
 * 
 * Represents a System V message queue with its identifier and state.
 */
typedef struct 
{
    int key;            /**< IPC key used to create/access the queue */
    int id;             /**< System V message queue identifier */
    bool initialized;   /**< Initialization state flag */

} sat_queue_ipc_t;

/**
 * @brief Message queue configuration structure
 * 
 * Configuration parameters for creating or accessing a message queue.
 */
typedef struct 
{
    int key;                        /**< IPC key (must be positive) */
    sat_queue_ipc_flags_t flags;    /**< Permission flags */

} sat_queue_ipc_args_t;

/**
 * @brief Message data structure
 * 
 * Represents a message to be sent or received through the queue.
 * The type field allows selective message reception.
 */
typedef struct 
{
    long int type;      /**< Message type (must be positive) */
    char *buffer;       /**< Message data buffer */
    size_t size;        /**< Message size in bytes (max 8192) */

} sat_queue_data_t;

/**
 * @brief Open or create a message queue
 * 
 * Creates a new System V message queue or opens an existing one based on
 * the specified key. If the queue already exists, it will be opened with
 * the current permissions.
 * 
 * The IPC_CREAT flag is automatically added, so the queue will be created
 * if it doesn't exist.
 * 
 * @param object Pointer to queue object to initialize
 * @param args Pointer to configuration structure with key and flags
 * @return Status indicating success or failure
 */
sat_status_t sat_queue_ipc_open (sat_queue_ipc_t *const object, const sat_queue_ipc_args_t *const args);

/**
 * @brief Send a message to the queue
 * 
 * Sends a message to the message queue. The message includes a type field
 * that can be used for selective reception. The function blocks if the
 * queue is full.
 * 
 * Maximum message size is 8192 bytes. Messages larger than this will be
 * rejected.
 * 
 * @param object Pointer to initialized queue object
 * @param data Pointer to message data structure with type, buffer, and size
 * @return Status indicating success or failure
 */
sat_status_t sat_queue_ipc_send (sat_queue_ipc_t *const object, const sat_queue_data_t *const data);

/**
 * @brief Receive a message from the queue
 * 
 * Receives a message from the queue matching the specified type. The function
 * blocks until a matching message is available.
 * 
 * The type field in the data structure determines which message to receive:
 * - type > 0: Receive the first message with the specified type
 * - type = 0: Receive the first message regardless of type
 * - type < 0: Receive the first message with lowest type <= |type|
 * 
 * The buffer must be pre-allocated with sufficient size.
 * 
 * @param object Pointer to initialized queue object
 * @param data Pointer to message data structure (type and buffer must be set)
 * @return Status indicating success or failure
 */
sat_status_t sat_queue_ipc_receive (sat_queue_ipc_t *const object, sat_queue_data_t *const data);

/**
 * @brief Destroy the message queue
 * 
 * Removes the message queue from the system. All messages are discarded
 * and the queue identifier becomes invalid. Other processes using the
 * queue will receive errors.
 * 
 * This should be called by one process (typically the creator) when the
 * queue is no longer needed. The queue persists in the kernel until
 * explicitly destroyed or system reboot.
 * 
 * @param object Pointer to initialized queue object
 * @return Status indicating success or failure
 */
sat_status_t sat_queue_ipc_destroy (sat_queue_ipc_t *const object);

#endif/* SAT_QUEUE_IPC_H_ */
