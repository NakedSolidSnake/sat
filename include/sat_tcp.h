#ifndef SAT_TCP_H_
#define SAT_TCP_H_

#include <sat_status.h>
#include <stdint.h>

#define SAT_TCP_HOSTNAME_SIZE       1024

typedef void (*sat_tcp_event_t) (char *buffer, uint32_t *size, void *data);

typedef enum 
{
    sat_tcp_type_server,
    sat_tcp_type_client
} sat_tcp_type_t;

typedef struct 
{
    int socket;
    sat_tcp_type_t type;
    char hostname [SAT_TCP_HOSTNAME_SIZE];
    const char *port;
    char *buffer;
    uint32_t size;
    struct 
    {
        sat_tcp_event_t on_receive;
        sat_tcp_event_t on_send;
    } events;
    void *data;
} sat_tcp_t;


typedef struct 
{
    sat_tcp_type_t type;
    char *hostname;
    const char *port;
    char *buffer;
    uint32_t size;
    struct 
    {
        sat_tcp_event_t on_receive;
        sat_tcp_event_t on_send;
    } events;
    void *data;
} sat_tcp_args_t;

sat_status_t sat_tcp_init (sat_tcp_t *object);
sat_status_t sat_tcp_open (sat_tcp_t *object, sat_tcp_args_t *args);
sat_status_t sat_tcp_run (sat_tcp_t *object);
sat_status_t sat_tcp_connect (sat_tcp_t *object);
sat_status_t sat_tcp_send (sat_tcp_t *object, const char *data, uint32_t size);
sat_status_t sat_tcp_receive (sat_tcp_t *object, char *data, uint32_t *size);
sat_status_t sat_tcp_close (sat_tcp_t *object);

#endif/* SAT_TCP_H_ */
