#ifndef SAT_TCP_SERVER_ABSTRACT_H_
#define SAT_TCP_SERVER_ABSTRACT_H_

#include <sat_tcp_types.h>
#include <sat_status.h>
#include <sat_tcp_server_base.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


typedef struct 
{
    int socket;
    const char *service;
    char *buffer;
    uint32_t size;
    struct 
    {
        sat_tcp_event_t on_receive;
        sat_tcp_event_t on_send;
    } events;

    void *data;

    sat_tcp_server_base_t base;

    sat_tcp_server_type_t type;

} sat_tcp_server_abstract_t;

void sat_tcp_server_abstract_copy_to_context (sat_tcp_server_abstract_t *object, sat_tcp_server_args_t *args);
sat_status_t sat_tcp_server_abstract_is_args_valid (sat_tcp_server_args_t *args);
sat_status_t sat_tcp_server_abstract_configure (sat_tcp_server_abstract_t *object, struct addrinfo *info_list);

#endif/* SAT_TCP_SERVER_ABSTRACT_H_ */
