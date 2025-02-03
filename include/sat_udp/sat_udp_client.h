#ifndef SAT_UDP_CLIENT_H_
#define SAT_UDP_CLIENT_H_

#include <sat_udp_types.h>
#include <sat_status.h>

sat_status_t sat_udp_client_open (sat_udp_client_t **object, sat_udp_client_args_t *args);
int sat_udp_client_get_socket (sat_udp_client_t *object);

#endif/* SAT_UDP_CLIENT_H_ */
