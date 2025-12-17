/**
 * @file sat_channel.h
 * @brief Inter-thread communication channel using pipes
 * 
 * This module provides a bidirectional communication channel for inter-thread
 * data exchange using Unix pipes. It allows threads to safely exchange data
 * through read and write operations on an internal pipe pair.
 * 
 * The channel is useful for producer-consumer patterns, thread synchronization,
 * and message passing between threads within the same process.
 */

#ifndef SAT_CHANNEL_H_
#define SAT_CHANNEL_H_

#include <sat_status.h>
#include <stdint.h>

/**
 * @brief Channel structure
 * 
 * Represents a bidirectional communication channel using a pipe pair.
 * This structure should be treated as opaque and accessed only through
 * the provided API functions.
 */
typedef struct
{
    int pair [2];           /**< Pipe file descriptor pair [read_fd, write_fd] */
    uint8_t *buffer;        /**< Internal buffer (reserved for future use) */
    uint32_t size;          /**< Buffer size (reserved for future use) */
} sat_channel_t;

/**
 * @brief Channel initialization arguments
 * 
 * Configuration parameters for opening a channel. Currently both fields
 * are reserved for future use and can be set to NULL/0.
 */
typedef struct
{
    uint8_t *buffer;        /**< Custom buffer (reserved for future use) */
    uint32_t size;          /**< Custom buffer size (reserved for future use) */
} sat_channel_args_t;

/**
 * @brief Initialize a channel
 * 
 * Initializes a channel structure by clearing all fields. This function must
 * be called before any other channel operations.
 * 
 * @param object Pointer to the channel structure to initialize
 * @return Status structure indicating success or failure
 * @note The channel is not ready for use until sat_channel_open() is called
 * @warning NULL pointer causes undefined behavior
 * @see sat_channel_open()
 */
sat_status_t sat_channel_init (sat_channel_t *const object);

/**
 * @brief Open a channel
 * 
 * Creates and opens a pipe pair for the channel, making it ready for
 * read and write operations. The pipe is created with default settings
 * suitable for inter-thread communication.
 * 
 * @param object Pointer to the initialized channel structure
 * @param args Pointer to initialization arguments (can be NULL or empty struct)
 * @return Status structure indicating success or failure
 * @note The channel must be initialized with sat_channel_init() first
 * @note The args parameter is currently unused; pass NULL or empty struct
 * @warning Fails if the system cannot create a pipe (e.g., resource limits)
 * @see sat_channel_init()
 * @see sat_channel_close()
 */
sat_status_t sat_channel_open (sat_channel_t *const object, const sat_channel_args_t *const args);

/**
 * @brief Write data to the channel
 * 
 * Writes data to the channel's write end. The data can be read by another
 * thread using sat_channel_read(). This operation blocks if the pipe buffer
 * is full.
 * 
 * @param object Pointer to the opened channel structure
 * @param buffer Pointer to the data to write
 * @param size Number of bytes to write
 * @return Status structure indicating success or failure
 * @note Write operations are atomic for sizes up to PIPE_BUF (typically 4096 bytes)
 * @note May block if the pipe buffer is full
 * @warning NULL buffer pointer causes undefined behavior
 * @see sat_channel_read()
 */
sat_status_t sat_channel_write (sat_channel_t *const object, const uint8_t *const buffer, uint32_t size);

/**
 * @brief Read data from the channel
 * 
 * Reads data from the channel's read end that was written by another thread.
 * This operation blocks if no data is available in the pipe.
 * 
 * @param object Pointer to the opened channel structure
 * @param buffer Pointer to buffer where data will be stored
 * @param size Maximum number of bytes to read
 * @return Status structure indicating success or failure
 * @note Blocks until data is available or the write end is closed
 * @note May read fewer bytes than requested if less data is available
 * @warning NULL buffer pointer causes undefined behavior
 * @warning Buffer must be large enough to hold size bytes
 * @see sat_channel_write()
 */
sat_status_t sat_channel_read (sat_channel_t *const object, uint8_t *const buffer, uint32_t size);

/**
 * @brief Close a channel
 * 
 * Closes both ends of the pipe and releases associated system resources.
 * After closing, the channel can be re-opened with sat_channel_open().
 * 
 * @param object Pointer to the opened channel structure
 * @return Status structure indicating success or failure
 * @note Any threads blocked on read/write operations will be unblocked
 * @note Pending data in the pipe buffer may be lost
 * @warning Always close channels to avoid resource leaks
 * @see sat_channel_open()
 */
sat_status_t sat_channel_close (sat_channel_t *const object);

#endif/* SAT_CHANNEL_H_ */
