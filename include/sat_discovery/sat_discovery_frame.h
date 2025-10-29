#ifndef SAT_DISCOVERY_FRAME_H
#define SAT_DISCOVERY_FRAME_H

#include <stdint.h>
#include <sat_status.h>
#include <sat_uuid.h>

#define SAT_DISCOVERY_FRAME_VERSION            1
#define SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE 32
#define SAT_DISCOVERY_FRAME_SIZE              56 


typedef enum
{
    sat_discovery_frame_type_announce = 1,
    sat_discovery_frame_type_interest = 2,
    sat_discovery_frame_type_heartbeat = 3,
    sat_discovery_frame_type_vanish = 4

} sat_discovery_frame_type_t;

/*

   Discovery Frame Header

   |version|uuid|type|
*/

typedef struct __attribute__ ((packed))
{
    uint8_t version;
    uuid_binary_t uuid;
    uint8_t type;
} sat_discovery_frame_header_t;

typedef struct __attribute__ ((packed))
{
    char service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE];
    uint16_t service_port;
    uint32_t address;
} sat_discovery_frame_announce_t;


typedef struct __attribute__ ((packed))
{
    char service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE];
} sat_discovery_frame_interest_t;

typedef struct __attribute__ ((packed))
{
    char service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE];
    uint64_t timestamp;
} sat_discovery_frame_heartbeat_t;

typedef struct __attribute__ ((packed))
{
    char service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE];
} sat_discovery_frame_vanish_t;


typedef struct __attribute__ ((packed))
{
    sat_discovery_frame_header_t header;
    
    union
    {
        sat_discovery_frame_announce_t announce;
        sat_discovery_frame_interest_t interest;
        sat_discovery_frame_heartbeat_t heartbeat;
        sat_discovery_frame_vanish_t vanish;
    } payload;
    
} sat_discovery_frame_t;

typedef struct __attribute__ ((packed))
{
    sat_discovery_frame_type_t type;
    uuid_binary_t uuid;
    char service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE];
    uint16_t service_port;
    uint32_t address;    
    
} sat_discovery_frame_args_t;

typedef struct __attribute__ ((packed))
{
    uint8_t data [SAT_DISCOVERY_FRAME_SIZE];
} sat_discovery_frame_buffer_t;

sat_status_t sat_discovery_frame_create (sat_discovery_frame_t *const object, sat_discovery_frame_args_t *args);
sat_status_t sat_discovery_frame_pack (const sat_discovery_frame_t *const object, sat_discovery_frame_buffer_t *const buffer);
sat_status_t sat_discovery_frame_unpack (sat_discovery_frame_t *const object, const sat_discovery_frame_buffer_t *const buffer);

#endif /* SAT_DISCOVERY_FRAME_H */
