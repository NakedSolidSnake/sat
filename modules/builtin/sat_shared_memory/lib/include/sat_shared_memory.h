#ifndef SAT_SHARED_MEMORY_H_
#define SAT_SHARED_MEMORY_H_

#include <sat_status.h>
#include <stdlib.h>

typedef enum 
{
    sat_shared_memory_flags_all_read_write = 0666
} sat_shared_memory_flags_t;

typedef struct 
{
    void *handle;
    bool initialized;
    int id;
    int key;
    size_t size;
} sat_shared_memory_t;

typedef struct 
{
    int key;
    size_t size;
    sat_shared_memory_flags_t flags;
} sat_shared_memory_args_t;

sat_status_t sat_shared_memory_open (sat_shared_memory_t *const object, const sat_shared_memory_args_t *const args);
sat_status_t sat_shared_memory_get (const sat_shared_memory_t *const object, void **const data);
sat_status_t sat_shared_memory_detach (const sat_shared_memory_t *const object);
sat_status_t sat_shared_memory_destroy (sat_shared_memory_t *const object);

#endif/* SAT_SHARED_MEMORY_H_ */
