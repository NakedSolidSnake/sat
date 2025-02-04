#include <sat_tcp_client.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct sat_tcp_client_t
{
    int socket;
    char hostname [SAT_TCP_HOSTNAME_SIZE];
    const char *service;
};

static struct addrinfo * sat_tcp_client_get_info_list (sat_tcp_client_args_t *args);

static sat_status_t sat_tcp_client_configure (sat_tcp_client_t *object, struct addrinfo *info_list);
static sat_status_t sat_tcp_client_set_socket (sat_tcp_client_t *object, struct addrinfo *info);
static sat_status_t sat_tcp_client_connect (sat_tcp_client_t *object, struct addrinfo *info);

sat_status_t sat_tcp_client_open (sat_tcp_client_t **object, sat_tcp_client_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp client open error");

    if (args->hostname != NULL && args->service != NULL)
    {
        do 
        {
            sat_tcp_client_t *__object = calloc (1, sizeof (sat_tcp_client_t));
            if (__object == NULL)
            {
                break;
            }

            struct addrinfo *info_list = sat_tcp_client_get_info_list (args);
            if (info_list == NULL)
            {
                break;
            }

            status = sat_tcp_client_configure (__object, info_list);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            __object->service = args->service;
            *object = __object;

        } while (false);
    }

    return status;
}

int sat_tcp_client_get_socket (sat_tcp_client_t *object)
{
    return object->socket;
}

static struct addrinfo *sat_tcp_client_get_info_list (sat_tcp_client_args_t *args)
{
    struct addrinfo hints;
    struct addrinfo *info_list = NULL;

    memset(&hints, 0, sizeof (hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo (args->hostname, args->service, &hints, &info_list);

    return info_list;
}

static sat_status_t sat_tcp_client_set_socket (sat_tcp_client_t *object, struct addrinfo *info)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp client set socket error");

    object->socket = socket (info->ai_family, info->ai_socktype, info->ai_protocol);
    if (object->socket >= 0)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_tcp_client_connect (sat_tcp_client_t *object, struct addrinfo *info)
{
    sat_status_t status = sat_status_set (&status, true, "");

    int status_connect = connect (object->socket, info->ai_addr, info->ai_addrlen);

    if ( status_connect < 0)
    {
        close (object->socket);

        sat_status_set (&status, false, "sat tcp client connect error");
    }

    return status;
}

static sat_status_t sat_tcp_client_configure (sat_tcp_client_t *object, struct addrinfo *info_list)
{
    sat_status_t status = sat_status_set (&status, true, "");

    struct addrinfo *info = NULL;

    for (info = info_list; info != NULL; info = info->ai_next)
    {
        status = sat_tcp_client_set_socket (object, info);
        if (sat_status_get_result (&status) == false)
        {
            continue;
        }

        status = sat_tcp_client_connect (object, info);
        if (sat_status_get_result (&status) == false)
        {
            continue;
        }

        break;
    }

    freeaddrinfo (info_list);

    return status;
}