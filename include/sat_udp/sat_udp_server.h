#ifndef SAT_UDP_SERVER_H_
#define SAT_UDP_SERVER_H_

#include <sat_udp.h>

sat_status_t sat_udp_server_open (sat_udp_t *object, sat_udp_args_t *args);
sat_status_t sat_udp_server_run (sat_udp_t *object);

#endif/* SAT_UDP_SERVER_H_ */

