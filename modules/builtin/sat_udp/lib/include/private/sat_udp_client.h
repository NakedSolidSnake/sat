#ifndef SAT_UDP_CLIENT_H
#define SAT_UDP_CLIENT_H

#include <sat_udp_types.h>
#include <sat_status.h>

sat_status_t sat_udp_client_open (sat_udp_client_t **const object, const sat_udp_client_args_t *const args);
int sat_udp_client_get_socket (const sat_udp_client_t *const object);
sat_status_t sat_udp_client_get_port (const sat_udp_client_t *const object, uint16_t *const port);

#endif /* SAT_UDP_CLIENT_H */
