/**
 * @file sat_cache.h
 * @brief Simple buffer caching mechanism for temporary data storage
 * @author Cristiano Silva de Souza
 * @date 2025
 * 
 * This module provides a lightweight caching mechanism for storing and retrieving
 * arbitrary data in a fixed-size buffer. It is useful for temporarily caching
 * computed results, network responses, or frequently accessed data.
 */

#ifndef SAT_CACHE_H_
#define SAT_CACHE_H_

#include <sat_status.h>
#include <stdint.h>

/**
 * @brief Cache object structure
 * 
 * Holds the state and data of a cache instance.
 */
typedef struct 
{
    /**
     * @brief Cached data information
     */
    struct
    {
        void *buffer;      /**< Pointer to the data buffer */
        uint32_t size;     /**< Size of the cached data in bytes */
    } data;

    bool is_cached;        /**< Flag indicating whether valid data is cached */
    
} sat_cache_t;

/**
 * @brief Configuration structure for cache creation
 * 
 * Contains parameters needed to initialize a cache instance.
 */
typedef struct 
{
    uint32_t buffer_size;  /**< Size of the buffer to allocate in bytes */
} sat_cache_args_t;

/**
 * @brief Initialize the cache object
 * 
 * Sets up the cache object by zeroing all fields. This function must be
 * called before any other cache operations.
 * 
 * @param[out] object Pointer to the cache object to initialize
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note This does not allocate the buffer. Use sat_cache_open() to allocate memory
 * @see sat_cache_open()
 */
sat_status_t sat_cache_init (sat_cache_t *const object);

/**
 * @brief Open the cache with specified arguments
 * 
 * Allocates the internal buffer according to the size specified in args.
 * The cache must be initialized with sat_cache_init() before calling this function.
 * 
 * @param[in,out] object Pointer to the cache object
 * @param[in] args Pointer to configuration structure containing buffer size
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning Buffer size must be greater than 0
 * @note The buffer remains allocated until sat_cache_close() is called
 * @see sat_cache_init(), sat_cache_close()
 */
sat_status_t sat_cache_open (sat_cache_t *const object, const sat_cache_args_t *const args);

/**
 * @brief Store data in the cache
 * 
 * Copies the provided data into the cache buffer and marks the cache as valid.
 * The data size must not exceed the buffer capacity.
 * 
 * @param[in,out] object Pointer to the cache object
 * @param[in] data Pointer to the data to store
 * @param[in] size Size of the data in bytes
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning Size must not exceed buffer_size specified during cache opening
 * @note Previous cached data is overwritten
 * @note The data is copied using memcpy
 */
sat_status_t sat_cache_store (sat_cache_t *const object, const void *const data, uint32_t size);

/**
 * @brief Restore data from the cache
 * 
 * Copies the cached data into the provided buffer. This operation only succeeds
 * if the cache contains valid data (is_cached flag is true).
 * 
 * @param[in] object Pointer to the cache object
 * @param[out] data Pointer to buffer where cached data will be copied
 * @param[in] size Size of the destination buffer in bytes
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning The destination buffer must be large enough to hold the cached data
 * @warning Fails if no data is currently cached
 * @see sat_cache_is_cached()
 */
sat_status_t sat_cache_restore (const sat_cache_t *const object, void *const data, uint32_t size);

/**
 * @brief Check if the cache contains valid data
 * 
 * Queries whether the cache currently holds valid cached data.
 * 
 * @param[in] object Pointer to the cache object
 * @return sat_status_t with result true if data is cached, false otherwise
 * 
 * @note The cache is considered valid after sat_cache_store() and invalid after sat_cache_clear()
 */
sat_status_t sat_cache_is_cached (const sat_cache_t *const object);

/**
 * @brief Clear the cached data
 * 
 * Invalidates the cached data by zeroing the buffer and setting the is_cached
 * flag to false. The buffer memory remains allocated.
 * 
 * @param[in,out] object Pointer to the cache object
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note This does not free the buffer memory, only marks the cache as empty
 * @note Subsequent calls to sat_cache_restore() will fail until new data is stored
 */
sat_status_t sat_cache_clear (sat_cache_t *const object);

/**
 * @brief Close the cache and free resources
 * 
 * Frees the allocated buffer memory and resets all cache fields to zero.
 * After calling this function, the cache object can be reused by calling
 * sat_cache_init() and sat_cache_open() again.
 * 
 * @param[in,out] object Pointer to the cache object
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note After calling this function, the cache object becomes invalid
 * @warning Any pointers to cached data become invalid after this call
 */
sat_status_t sat_cache_close (sat_cache_t *const object);

#endif/* SAT_CACHE_H_ */
