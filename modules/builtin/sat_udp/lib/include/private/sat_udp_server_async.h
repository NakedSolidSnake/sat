#ifndef SAT_UDP_SERVER_ASYNC_H_
#define SAT_UDP_SERVER_ASYNC_H_

#include <sat_udp_server_base.h>
#include <sat_udp_server_abstract.h>
#include <sys/select.h>

typedef struct 
{
    sat_udp_server_abstract_t abstract;

    fd_set master;
    fd_set read;
    
    int max_socket;

} sat_udp_server_async_t;

sat_udp_server_base_t *sat_udp_server_async_create (void);

#endif/* SAT_UDP_SERVER_ASYNC_H_ */
