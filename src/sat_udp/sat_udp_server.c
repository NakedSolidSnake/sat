#include <sat_udp_server.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

static sat_status_t sat_udp_server_is_args_valid (sat_udp_args_t *args);
static sat_status_t sat_udp_server_configure (sat_udp_t *object, struct addrinfo *info_list);
static sat_status_t sat_udp_server_set_socket (sat_udp_t *object, struct addrinfo *info);
static sat_status_t sat_udp_server_set_reuse_address (sat_udp_t *object);
static sat_status_t sat_udp_server_set_bind (sat_udp_t *object, struct addrinfo *info);
static struct addrinfo *sat_udp_server_get_info_list (sat_udp_args_t *args);

sat_status_t sat_udp_server_open (sat_udp_t *object, sat_udp_args_t *args)
{
    sat_status_t status = sat_udp_server_is_args_valid (args);

    do 
    {
        if (sat_status_get_result (&status) == false)
        {
            break;
        }
        
        object->buffer = args->buffer;
        object->size = args->size;
        object->service = args->service;
        object->events.on_receive = args->events.on_receive;
        object->events.on_send = args->events.on_send;
        object->data = args->data;

        struct addrinfo *info_list = sat_udp_server_get_info_list (args);

        if (info_list == NULL)
        {
            break;
        }

        status = sat_udp_server_configure (object, info_list);

        freeaddrinfo (info_list);

    } while (false);

    return status;
}

sat_status_t sat_udp_server_run (sat_udp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server args error");

    struct sockaddr_storage source;
    socklen_t length = sizeof (source);
    uint32_t size;

    memset (&source, 0, sizeof (struct sockaddr_storage));

    if (object->socket >= 0)
    {
        size = recvfrom (object->socket,
                         object->buffer,
                         object->size,
                         0,
                         (struct sockaddr *)&source,
                         &length);

        object->buffer [size] = 0;

        if (object->events.on_receive)
            object->events.on_receive (object->buffer, &size, object->data);

        if (object->events.on_send)
        {
            object->events.on_send (object->buffer, &size, object->data);
            size = size > object->size ? object->size : size;
            sendto (object->socket,
                    object->buffer,
                    size,
                    0,
                    (struct sockaddr *)&source,
                    sizeof (source));                    
        }

        memset (object->buffer, 0, object->size);

        sat_status_set (&status, true, "");
    }


    return status;
}

static sat_status_t sat_udp_server_is_args_valid (sat_udp_args_t *args)
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

static sat_status_t sat_udp_server_configure (sat_udp_t *object, struct addrinfo *info_list)
{
    sat_status_t status;

    struct addrinfo *info = NULL;

    for (info = info_list; info != NULL; info = info->ai_next)
    {
        status = sat_udp_server_set_socket (object, info);

        if (sat_status_get_result (&status) == false)
            continue;

        status = sat_udp_server_set_reuse_address (object);
        if (sat_status_get_result (&status) == false)
            break;

        status = sat_udp_server_set_bind (object, info);
        if (sat_status_get_result (&status) == false)
        {
            close (object->socket);
            continue;
        }

        break;
    }

    return status;
}

static sat_status_t sat_udp_server_set_socket (sat_udp_t *object, struct addrinfo *info)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server set socket error");

    object->socket = socket (info->ai_family, info->ai_socktype, info->ai_protocol);

    if (object->socket >= 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_udp_server_set_reuse_address (sat_udp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server set reuse address error");
    int yes = 1;

    if (setsockopt (object->socket, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof (yes)) == 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_udp_server_set_bind (sat_udp_t *object, struct addrinfo *info)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server set bind error");

    if (bind (object->socket, info->ai_addr, info->ai_addrlen) == 0)
        sat_status_set (&status, true, "");

    return status;
}

static struct addrinfo *sat_udp_server_get_info_list (sat_udp_args_t *args)
{
    struct addrinfo hints;
    struct addrinfo *info_list = NULL;

    memset(&hints, 0, sizeof (hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_UDP;

    getaddrinfo (NULL, args->service, &hints, &info_list);

    return info_list;
}