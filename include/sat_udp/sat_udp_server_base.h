#ifndef SAT_UDP_SERVER_BASE_H_
#define SAT_UDP_SERVER_BASE_H_

#include <sat_status.h>
#include <sat_udp_types.h>

typedef struct 
{
    void *object;
    sat_status_t (*open) (void *object, sat_udp_server_args_t *args);
    sat_status_t (*run) (void *object);
    int (*get_socket) (void *object);

} sat_udp_server_base_t;

#endif/* SAT_UDP_SERVER_BASE_H_ */
