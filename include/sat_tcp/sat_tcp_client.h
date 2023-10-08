#ifndef SAT_TCP_CLIENT_H_
#define SAT_TCP_CLIENT_H_

#include <sat_tcp.h>

sat_status_t sat_tcp_client_open (sat_tcp_t *object, sat_tcp_args_t *args);
sat_status_t sat_tcp_client_connect (sat_tcp_t *object);

#endif/* SAT_TCP_CLIENT_H_ */
