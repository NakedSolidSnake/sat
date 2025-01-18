#ifndef SAT_UDP_H_
#define SAT_UDP_H_

#include <sat_status.h>
#include <stdint.h>

#define SAT_UDP_HOSTNAME_SIZE       1024

typedef void (*sat_udp_event_t) (char *buffer, uint32_t *size, void *data);

typedef enum 
{
    sat_udp_type_server,
    sat_udp_type_client
} sat_udp_type_t;

typedef struct 
{
    int socket;
    sat_udp_type_t type;
    char hostname [SAT_UDP_HOSTNAME_SIZE];
    const char *service;
    char *buffer;
    uint32_t size;
    struct 
    {
        sat_udp_event_t on_receive;
        sat_udp_event_t on_send;
    } events;
    void *data;
} sat_udp_t;


typedef struct 
{
    sat_udp_type_t type;
    char *hostname;
    const char *service;
    char *buffer;
    uint32_t size;
    struct 
    {
        sat_udp_event_t on_receive;
        sat_udp_event_t on_send;
    } events;
    void *data;
} sat_udp_args_t;

sat_status_t sat_udp_init (sat_udp_t *object);
sat_status_t sat_udp_open (sat_udp_t *object, sat_udp_args_t *args);
sat_status_t sat_udp_run (sat_udp_t *object);
sat_status_t sat_udp_send (sat_udp_t *object, const char *data, uint32_t size);
sat_status_t sat_udp_receive (sat_udp_t *object, char *data, uint32_t *size);
sat_status_t sat_udp_close (sat_udp_t *object);

#endif/* SAT_UDP_H_ */

