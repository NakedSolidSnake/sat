#ifndef SAT_CHANNEL_H_
#define SAT_CHANNEL_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct
{
    int pair [2];
    uint8_t *buffer;
    uint32_t size;
} sat_channel_t;

typedef struct
{
    uint8_t *buffer;
    uint32_t size;
} sat_channel_args_t;

sat_status_t sat_channel_init (sat_channel_t *object);
sat_status_t sat_channel_open (sat_channel_t *object, sat_channel_args_t *args);
sat_status_t sat_channel_write (sat_channel_t *object, uint8_t *buffer, uint32_t size);
sat_status_t sat_channel_read (sat_channel_t *object, uint8_t *buffer, uint32_t size);
sat_status_t sat_channel_close (sat_channel_t *object);

#endif/* SAT_CHANNEL_H_ */
