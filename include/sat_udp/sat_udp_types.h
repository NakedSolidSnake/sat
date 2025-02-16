#ifndef SAT_UDP_TYPES_H_
#define SAT_UDP_TYPES_H_

#include <stdint.h>

#define SAT_UDP_HOSTNAME_SIZE       1024

typedef void (*sat_udp_event_t) (char *buffer, uint32_t *size, void *data);

typedef enum 
{
    sat_udp_type_server,
    sat_udp_type_client

} sat_udp_type_t;

typedef enum 
{
    sat_udp_server_type_interactive,
    sat_udp_server_type_async,

} sat_udp_server_type_t;

typedef enum 
{
    sat_udp_server_mode_unicast,
    sat_udp_server_mode_multicast,

} sat_udp_server_mode_t;

typedef struct sat_udp_server_t sat_udp_server_t;
typedef struct sat_udp_client_t sat_udp_client_t;

typedef struct 
{
    const char *multicast_group;
    const char *service;
    char *buffer;
    uint32_t size;

    struct 
    {
        sat_udp_event_t on_receive;
        sat_udp_event_t on_send;
        
    } events;

    void *data;

    sat_udp_server_type_t type;

    sat_udp_server_mode_t mode;

} sat_udp_server_args_t;

typedef struct 
{
    char *hostname;
    const char *service;

} sat_udp_client_args_t;

#endif/* SAT_UDP_TYPES_H_ */
