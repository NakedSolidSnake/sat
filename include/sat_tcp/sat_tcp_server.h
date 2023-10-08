#ifndef SAT_TCP_SERVER_H_
#define SAT_TCP_SERVER_H_

#include <sat_tcp.h>

sat_status_t sat_tcp_server_open (sat_tcp_t *object, sat_tcp_args_t *args);
sat_status_t sat_tcp_server_run (sat_tcp_t *object);

#endif/* SAT_TCP_SERVER_H_ */
