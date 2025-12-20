#ifndef SAT_TCP_CLIENT_H_
#define SAT_TCP_CLIENT_H_

#include <sat_tcp_types.h>
#include <sat_status.h>

sat_status_t sat_tcp_client_open (sat_tcp_client_t **object, sat_tcp_client_args_t *args);
int sat_tcp_client_get_socket (sat_tcp_client_t *object);

#endif/* SAT_TCP_CLIENT_H_ */
