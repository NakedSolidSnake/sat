#include <sat_udp_server_abstract.h>
#include <sat_udp_server_interactive.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

static sat_status_t sat_udp_server_abstract_set_socket (sat_udp_server_abstract_t *object, struct addrinfo *info);
static sat_status_t sat_udp_server_abstract_set_reuse_address (sat_udp_server_abstract_t *object);
static sat_status_t sat_udp_server_abstract_set_bind (sat_udp_server_abstract_t *object, struct addrinfo *info);

static sat_status_t sat_udp_server_abstract_select_type (sat_udp_server_abstract_t *object);

void sat_udp_server_abstract_copy_to_context (sat_udp_server_abstract_t *object, sat_udp_server_args_t *args)
{
    object->buffer = args->buffer;
    object->size = args->size;
    object->service = args->service;
    object->events.on_receive = args->events.on_receive;
    object->events.on_send = args->events.on_send;
    object->data = args->data;
}

sat_status_t sat_udp_server_abstract_is_args_valid (sat_udp_server_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server args error");

    if (args->buffer != NULL &&
        args->size > 0 && 
        args->service != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_udp_server_abstract_configure (sat_udp_server_abstract_t *object, struct addrinfo *info_list)
{
    sat_status_t status;

    struct addrinfo *info = NULL;

    for (info = info_list; info != NULL; info = info->ai_next)
    {
        status = sat_udp_server_abstract_set_socket (object, info);
        if (sat_status_get_result (&status) == false)
            continue;

        status = sat_udp_server_abstract_set_reuse_address (object);
        if (sat_status_get_result (&status) == false)
            break;

        status = sat_udp_server_abstract_set_bind (object, info);
        if (sat_status_get_result (&status) == false)
        {
            close (object->socket);
            continue;
        }

        status = sat_udp_server_abstract_select_type (object);

        break;
    }

    return status;
}

static sat_status_t sat_udp_server_abstract_set_socket (sat_udp_server_abstract_t *object, struct addrinfo *info)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server abstract set socket error");

    object->socket = socket (info->ai_family, info->ai_socktype, info->ai_protocol);

    if (object->socket >= 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_udp_server_abstract_set_reuse_address (sat_udp_server_abstract_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server abstract set reuse address error");
    int yes = 1;

    if (setsockopt (object->socket, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof (yes)) == 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_udp_server_abstract_set_bind (sat_udp_server_abstract_t *object, struct addrinfo *info)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server abstract set bind error");

    if (bind (object->socket, info->ai_addr, info->ai_addrlen) == 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_udp_server_abstract_select_type (sat_udp_server_abstract_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server abstract select type error");
    
    if (object->type == sat_udp_server_type_interactive)
    {

        object->base = sat_udp_server_interactive_create ();

        sat_status_set (&status, true, "");
    }

    return status;
}