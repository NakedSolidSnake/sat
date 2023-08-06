#include <sat_udp_server.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static sat_status_t sat_udp_server_is_args_valid (sat_udp_args_t *args);
static sat_status_t sat_udp_server_configure (sat_udp_t *object);
static sat_status_t sat_udp_server_set_socket (sat_udp_t *object);
static sat_status_t sat_udp_server_set_reuse_address (sat_udp_t *object);
static sat_status_t sat_udp_server_set_bind (sat_udp_t *object);


sat_status_t sat_udp_server_open (sat_udp_t *object, sat_udp_args_t *args)
{
    sat_status_t status = sat_udp_server_is_args_valid (args);

    if (sat_status_get_result (&status) == true)
    {
        object->buffer = args->buffer;
        object->size = args->size;
        object->port = args->port;
        object->events.on_receive = args->events.on_receive;
        object->events.on_send = args->events.on_send;
        object->data = args->data;

        status = sat_udp_server_configure (object);
    }

    return status;
}

sat_status_t sat_udp_server_run (sat_udp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server args error");

    struct sockaddr_in address_in;
    socklen_t length = sizeof (address_in);
    uint32_t size;

    if (object->socket >= 0)
    {
        size = recvfrom (object->socket,
                         object->buffer,
                         object->size,
                         MSG_WAITALL,
                         (struct sockaddr *)&address_in,
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
                    (struct sockaddr *)&address_in,
                    sizeof (address_in));                    
        }

        memset (&address_in, 0, sizeof (struct sockaddr_in));
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
        args->port != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_udp_server_configure (sat_udp_t *object)
{
    sat_status_t status;

    do 
    {
        status = sat_udp_server_set_socket (object);
        if (sat_status_get_result (&status) == false)
            break;

        status = sat_udp_server_set_reuse_address (object);
        if (sat_status_get_result (&status) == false)
            break;

        status = sat_udp_server_set_bind (object);

    } while (false);

    return status;
}

static sat_status_t sat_udp_server_set_socket (sat_udp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server set socket error");

    object->socket = socket (AF_INET, SOCK_DGRAM, 0);
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

static sat_status_t sat_udp_server_set_bind (sat_udp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server set bind error");

    struct sockaddr_in address;

    memset (&address, 0, sizeof (struct sockaddr_in));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (atoi (object->port));

    if (bind (object->socket, (const struct sockaddr *)&address, sizeof (struct sockaddr_in)) == 0)
        sat_status_set (&status, true, "");

    return status;
}