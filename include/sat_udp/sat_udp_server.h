#ifndef SAT_UDP_SERVER_H_
#define SAT_UDP_SERVER_H_

// #include <sat_udp.h>
#include <sat_udp_types.h>
#include <sat_status.h>

sat_status_t sat_udp_server_open (sat_udp_server_t **object, sat_udp_server_args_t *args);
sat_status_t sat_udp_server_run (sat_udp_server_t *object);
int sat_udp_server_get_socket (sat_udp_server_t *object);

#endif/* SAT_UDP_SERVER_H_ */

