#ifndef SAT_TCP_TYPES_H_
#define SAT_TCP_TYPES_H_

#include <stdint.h>

#define SAT_TCP_HOSTNAME_SIZE       1024

typedef void (*sat_tcp_event_t) (char *buffer, uint32_t *size, void *data);

typedef enum 
{
    sat_tcp_type_server,
    sat_tcp_type_client
} sat_tcp_type_t;

typedef enum 
{
    sat_tcp_server_type_interactive,

} sat_tcp_server_type_t;

typedef struct sat_tcp_server_t sat_tcp_server_t;
typedef struct sat_tcp_client_t sat_tcp_client_t;

typedef struct 
{
    const char *service;
    char *buffer;
    uint32_t size;

    struct 
    {
        sat_tcp_event_t on_receive;
        sat_tcp_event_t on_send;
    } events;

    void *data;

    sat_tcp_server_type_t type;

} sat_tcp_server_args_t;

typedef struct 
{
    char *hostname;
    const char *service;
} sat_tcp_client_args_t;

#endif/* SAT_TCP_TYPES_H_ */
