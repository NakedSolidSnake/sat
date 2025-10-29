#ifndef SAT_DISCOVERY_FRAME_H
#define SAT_DISCOVERY_FRAME_H

#include <stdint.h>
#include <sat_status.h>

#define SAT_DISCOVERY_FRAME_VERSION            1
#define SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE 32
#define SAT_DISCOVERY_FRAME_SIZE              48

typedef enum : uint8_t
{
    sat_discovery_frame_type_announce = 1,
    sat_discovery_frame_type_interest = 2,
    sat_discovery_frame_type_heartbeat = 3,

} sat_discovery_frame_type_t;

typedef struct __attribute__ ((packed))
{
    uint8_t version;
    sat_discovery_frame_type_t type;
    uint32_t flags;
    uint32_t timestamp;
    char service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE];
    uint16_t service_port;
    uint32_t address;    
    
} sat_discovery_frame_t;

typedef struct __attribute__ ((packed))
{
    uint8_t version;
    sat_discovery_frame_type_t type;
    uint32_t flags;
    uint32_t timestamp;
    char service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE];
    uint16_t service_port;
    uint32_t address;    
    
} sat_discovery_frame_args_t;

typedef struct
{
    uint8_t buffer [sizeof (sat_discovery_frame_t)];
    uint8_t size;
} sat_discovery_frame_buffer_t;

sat_status_t sat_discover_frame_create (sat_discovery_frame_t *const object, sat_discovery_frame_args_t *args);
sat_status_t sat_discover_frame_pack (const sat_discovery_frame_t *const object, sat_discovery_frame_buffer_t *const buffer);
sat_status_t sat_discover_frame_unpack (sat_discovery_frame_t *const object, const sat_discovery_frame_buffer_t *const buffer);

#endif /* SAT_DISCOVERY_FRAME_H */
