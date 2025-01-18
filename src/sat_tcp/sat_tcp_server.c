#include <sat_tcp_server.h>
#include <sat_tcp_server_abstract.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>

struct sat_tcp_server_t
{
    sat_tcp_server_abstract_t abstract;
};

static struct addrinfo *sat_tcp_server_get_info_list (sat_tcp_server_args_t *args);

sat_status_t sat_tcp_server_open (sat_tcp_server_t **object, sat_tcp_server_args_t *args)
{
    sat_status_t status;

    do
    {
        status = sat_tcp_server_abstract_is_args_valid (args);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        sat_tcp_server_t *__object = calloc (1, sizeof (sat_tcp_server_t));
        if (__object == NULL)
        {
            break;
        }

        sat_tcp_server_abstract_copy_to_context (&__object->abstract, args);

        struct addrinfo *info_list = sat_tcp_server_get_info_list (args);

        if (info_list == NULL)
        {
            free (__object);
            break;
        }

        status = sat_tcp_server_abstract_configure (&__object->abstract, info_list);
        if (sat_status_get_result (&status) == false)
        {
            freeaddrinfo (info_list);
            free (__object);
            break;
        }

        freeaddrinfo (info_list);

        *object = __object;

    } while (false);

    return status;
}

sat_status_t sat_tcp_server_run (sat_tcp_server_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp server run error");

    struct sockaddr_in address_in;
    socklen_t length = sizeof (address_in);

    if (object->abstract.socket >= 0)
    {
        int client_accept = accept (object->abstract.socket, (struct sockaddr *)&address_in, &length);

        status = object->abstract.base.handle_client (&object->abstract, client_accept);
    }

    return status;
}

int sat_tcp_server_get_socket (sat_tcp_server_t *object)
{
    return object->abstract.socket;
}

static struct addrinfo *sat_tcp_server_get_info_list (sat_tcp_server_args_t *args)
{
    struct addrinfo hints;
    struct addrinfo *info_list = NULL;

    memset(&hints, 0, sizeof (hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo (NULL, args->service, &hints, &info_list);

    return info_list;
}
