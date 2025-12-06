#ifndef SAT_CACHE_H_
#define SAT_CACHE_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct 
{
    uint32_t buffer_size;
    void *buffer;
    bool is_cached;
} sat_cache_t;

typedef struct 
{
    uint32_t buffer_size;
} sat_cache_args_t;

/**
 * @brief Initialize the cache object.
 * @details This function sets up the cache object for use.
 * @param object 
 * @return sat_status_t 
 */
sat_status_t sat_cache_init (sat_cache_t *const object);


/**
 * @brief Open the cache with specified arguments.
 * @details This function sets up the cache object with the provided arguments.
 * @param object 
 * @param args 
 * @return sat_status_t 
 */
sat_status_t sat_cache_open (sat_cache_t *const object, const sat_cache_args_t *const args);

/**
 * @brief Store data in the cache.
 * 
 * @param object 
 * @param data 
 * @param size 
 * @return sat_status_t 
 */
sat_status_t sat_cache_store (sat_cache_t *const object, const void *const data, uint32_t size);
sat_status_t sat_cache_restore (const sat_cache_t *const object, void *const data, uint32_t size);
sat_status_t sat_cache_is_cached (const sat_cache_t *const object);
sat_status_t sat_cache_clear (sat_cache_t *const object);
sat_status_t sat_cache_close (sat_cache_t *const object);

#endif/* SAT_CACHE_H_ */
