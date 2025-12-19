#ifndef SAT_UDP_SERVER_BASE_H_
#define SAT_UDP_SERVER_BASE_H_

#include <sat_status.h>
#include <sat_udp_types.h>

typedef struct 
{
    void *object;
    sat_status_t (*open) (void *const object, const sat_udp_server_args_t *const args);
    sat_status_t (*run) (void *const object);
    int (*get_socket) (const void *const object);

} sat_udp_server_base_t;

#endif/* SAT_UDP_SERVER_BASE_H_ */
