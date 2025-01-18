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

sat_status_t sat_cache_init (sat_cache_t *object);
sat_status_t sat_cache_open (sat_cache_t *object, sat_cache_args_t *args);
sat_status_t sat_cache_store (sat_cache_t *object, void *data, uint32_t size);
sat_status_t sat_cache_restore (sat_cache_t *object, void *data, uint32_t size);
sat_status_t sat_cache_is_cached (sat_cache_t *object);
sat_status_t sat_cache_clear (sat_cache_t *object);
sat_status_t sat_cache_close (sat_cache_t *object);

#endif/* SAT_CACHE_H_ */
