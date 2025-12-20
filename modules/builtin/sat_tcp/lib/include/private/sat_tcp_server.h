#ifndef SAT_TCP_SERVER_H_
#define SAT_TCP_SERVER_H_

#include <sat_tcp_types.h>
#include <sat_status.h>

sat_status_t sat_tcp_server_open (sat_tcp_server_t **object, sat_tcp_server_args_t *args);
sat_status_t sat_tcp_server_run (sat_tcp_server_t *object);
int sat_tcp_server_get_socket (sat_tcp_server_t *object);

#endif/* SAT_TCP_SERVER_H_ */
