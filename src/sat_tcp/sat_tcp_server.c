#include <sat_tcp_server.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>

static sat_status_t sat_tcp_server_is_args_valid (sat_tcp_args_t *args);
static sat_status_t sat_tcp_server_configure (sat_tcp_t *object);
static sat_status_t sat_tcp_server_set_socket (sat_tcp_t *object);
static sat_status_t sat_tcp_server_set_reuse_address (sat_tcp_t *object);
static sat_status_t sat_tcp_server_set_bind (sat_tcp_t *object);
static sat_status_t sat_tcp_server_listen (sat_tcp_t *object);
static sat_status_t sat_tcp_server_handle_client (sat_tcp_t *object, int client);



sat_status_t sat_tcp_server_open (sat_tcp_t *object, sat_tcp_args_t *args)
{
    sat_status_t status = sat_tcp_server_is_args_valid (args);

    if (sat_status_get_result (&status) == true)
    {
        object->buffer = args->buffer;
        object->size = args->size;
        object->port = args->port;
        object->events.on_receive = args->events.on_receive;
        object->events.on_send = args->events.on_send;
        object->data = args->data;

        status = sat_tcp_server_configure (object);
    }

    return status;
}

sat_status_t sat_tcp_server_run (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp server run error");

    struct sockaddr_in address_in;
    socklen_t length = sizeof (address_in);

    if (object->socket >= 0)
    {
        int client_accept = accept (object->socket, (struct sockaddr *)&address_in, &length);

        status = sat_tcp_server_handle_client (object, client_accept);
    }

    return status;
}

static sat_status_t sat_tcp_server_is_args_valid (sat_tcp_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp server args error");

    if (args->buffer != NULL &&
        args->size > 0 && 
        args->port != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_tcp_server_configure (sat_tcp_t *object)
{
    sat_status_t status;

    do 
    {
        status = sat_tcp_server_set_socket (object);
        if (sat_status_get_result (&status) == false)
            break;

        status = sat_tcp_server_set_reuse_address (object);
        if (sat_status_get_result (&status) == false)
            break;

        status = sat_tcp_server_set_bind (object);
        if (sat_status_get_result (&status) == false)
            break;

        status = sat_tcp_server_listen (object);

    } while (false);

    return status;
}

static sat_status_t sat_tcp_server_set_socket (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp server set socket error");

    object->socket = socket (AF_INET, SOCK_STREAM, 0);
    if (object->socket >= 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_tcp_server_set_reuse_address (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp server set reuse address error");
    int yes = 1;

    if (setsockopt (object->socket, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof (yes)) == 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_tcp_server_set_bind (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp server set bind error");

    struct sockaddr_in address;

    memset (&address, 0, sizeof (struct sockaddr_in));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (atoi (object->port));

    if (bind (object->socket, (const struct sockaddr *)&address, sizeof (struct sockaddr_in)) == 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_tcp_server_listen (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp server listen error");
    
    if (listen (object->socket, 1) >= 0)
        sat_status_set (&status, true, "");
    return status;
}

static sat_status_t sat_tcp_server_handle_client (sat_tcp_t *object, int client)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp server handle client error");
    
    uint32_t size;

    if (client > 0)
    {
        while (true)
        {
            memset (object->buffer, 0, object->size);

            size = recv (client, object->buffer, object->size, 0);

            if (size == 0)
                break;

            else if (object->events.on_receive != NULL && size > 0)
            {
                object->events.on_receive (object->buffer, &size, object->data);

                if (object->events.on_send != NULL)
                {
                    object->events.on_send (object->buffer, &size, object->data);
                    send (client, object->buffer, (int)fmin (size, object->size), 0);
                }
            }
        }

        sat_status_set (&status, true, "");
    }
    
    return status;
}