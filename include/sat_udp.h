#ifndef SAT_UDP_H_
#define SAT_UDP_H_

#include <sat_status.h>
#include <stdint.h>
#include <sat_udp/sat_udp_types.h>

typedef struct 
{
    sat_udp_type_t type;
    sat_udp_server_t *server;
    sat_udp_client_t *client;

} sat_udp_t;

typedef struct 
{
    sat_udp_type_t type;
    sat_udp_server_args_t server;
    sat_udp_client_args_t client;
    
} sat_udp_args_t;

typedef struct 
{
    const char *hostname;
    const char *service;

} sat_udp_destination_t;

sat_status_t sat_udp_init (sat_udp_t *object);
sat_status_t sat_udp_open (sat_udp_t *object, sat_udp_args_t *args);
sat_status_t sat_udp_run (sat_udp_t *object);
sat_status_t sat_udp_send (sat_udp_t *object, const char *data, uint32_t size, sat_udp_destination_t *destination);
sat_status_t sat_udp_receive (sat_udp_t *object, char *data, uint32_t *size, int timeout_ms);
sat_status_t sat_udp_close (sat_udp_t *object);
sat_status_t sat_udp_get_port (sat_udp_t *object, uint16_t *port);

#endif/* SAT_UDP_H_ */

