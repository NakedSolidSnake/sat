#ifndef SAT_UDP_SERVER_BASE_H_
#define SAT_UDP_SERVER_BASE_H_

#include <sat_status.h>

typedef struct 
{
    sat_status_t (*run) (void *object);

} sat_udp_server_base_t;

#endif/* SAT_UDP_SERVER_BASE_H_ */
