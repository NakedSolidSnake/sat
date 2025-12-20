#ifndef SAT_TCP_H_
#define SAT_TCP_H_

#include <sat_status.h>
#include <stdint.h>
#include <sat_tcp_types.h>

typedef struct 
{
    sat_tcp_type_t type;
    sat_tcp_server_t *server;
    sat_tcp_client_t *client;

} sat_tcp_t;


typedef struct 
{
    sat_tcp_type_t type;
    sat_tcp_server_args_t server;
    sat_tcp_client_args_t client;

} sat_tcp_args_t;

sat_status_t sat_tcp_init (sat_tcp_t *object);
sat_status_t sat_tcp_open (sat_tcp_t *object, sat_tcp_args_t *args);
sat_status_t sat_tcp_run (sat_tcp_t *object);
sat_status_t sat_tcp_send (sat_tcp_t *object, const char *data, uint32_t size);
sat_status_t sat_tcp_receive (sat_tcp_t *object, char *data, uint32_t *size);
sat_status_t sat_tcp_close (sat_tcp_t *object);

#endif/* SAT_TCP_H_ */
