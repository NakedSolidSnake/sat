#ifndef SAT_UDP_SERVER_ABSTRACT_H_
#define SAT_UDP_SERVER_ABSTRACT_H_

#include <sat_udp_types.h>
#include <sat_udp_server_base.h>
#include <sat_status.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef struct 
{
    sat_udp_server_base_t base;
    int socket;
    const char *service;
    char *buffer;
    uint32_t size;
    struct 
    {
        sat_udp_event_t on_receive;
        sat_udp_event_t on_send;
        sat_udp_multicast_event_t on_multicast_join;
    } events;

    void *data;

    sat_udp_server_type_t type;

} sat_udp_server_abstract_t;

sat_status_t sat_udp_server_abstract_open (sat_udp_server_abstract_t *object, sat_udp_server_args_t *args);
int sat_udp_server_abstract_get_socket (sat_udp_server_abstract_t *object);

#endif/* SAT_UDP_SERVER_ABSTRACT_H_ */
