#ifndef SAT_REDIS_H_
#define SAT_REDIS_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct 
{
    void *handle;
} sat_redis_t;

typedef struct 
{
    char *host;
    uint16_t port;
} sat_redis_args_t;

typedef void (*sat_redis_on_read_t) (char *content, void *data);

sat_status_t sat_redis_init (sat_redis_t *object);
sat_status_t sat_redis_open (sat_redis_t *object, sat_redis_args_t *args);
sat_status_t sat_redis_save (sat_redis_t *object, char *key, char *value);
sat_status_t sat_redis_get (sat_redis_t *object, char *key, sat_redis_on_read_t on_read, void *data);
sat_status_t sat_redis_is_key_exists (sat_redis_t *object, char *key);
sat_status_t sat_redis_close (sat_redis_t *object);

#endif/* SAT_REDIS_H_ */
