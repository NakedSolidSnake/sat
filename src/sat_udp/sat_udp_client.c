#include <sat_udp_client.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct sat_udp_client_t
{
    int socket;
    char hostname [SAT_UDP_HOSTNAME_SIZE + 1];
    const char *service;
};

static struct addrinfo * sat_udp_client_get_info_list (sat_udp_client_args_t *args);

static sat_status_t sat_udp_client_configure (sat_udp_client_t *object, struct addrinfo *info_list);
static sat_status_t sat_udp_client_set_socket (sat_udp_client_t *object, struct addrinfo *info);
static sat_status_t sat_udp_client_get_ip_by_hostname (sat_udp_client_t *object, sat_udp_client_args_t *args);


sat_status_t sat_udp_client_open (sat_udp_client_t **object, sat_udp_client_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp client open error");

    do 
    {
        sat_udp_client_t *__object = calloc (1, sizeof (sat_udp_client_t));
        if (__object == NULL)
        {
            break;
        }

        status = sat_udp_client_set_socket (__object, NULL);
        if (sat_status_get_result (&status) == false)
        {
            continue;
        }

        __object->service = args->service;
        *object = __object;

    } while (false);

    return status;
}

int sat_udp_client_get_socket (sat_udp_client_t *object)
{
    return object->socket;
}

static struct addrinfo *sat_udp_client_get_info_list (sat_udp_client_args_t *args)
{
    struct addrinfo hints;
    struct addrinfo *info_list = NULL;

    memset(&hints, 0, sizeof (hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_UDP;
    

    getaddrinfo (args->hostname, args->service, &hints, &info_list);

    return info_list;
}

static sat_status_t sat_udp_client_set_socket (sat_udp_client_t *object, struct addrinfo *info)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp client set socket error");


    object->socket = socket( AF_INET6, SOCK_DGRAM, 0);
    if (object->socket >= 0)
    {
        sat_status_set (&status, true, "");
    }

    int disable_v6_only = 0;
    if (setsockopt (object->socket, IPPROTO_IPV6, IPV6_V6ONLY, &disable_v6_only, sizeof(disable_v6_only)) < 0)
    {
        exit(EXIT_FAILURE);
    }

    return status;
}

static sat_status_t sat_udp_client_configure (sat_udp_client_t *object, struct addrinfo *info_list)
{
    sat_status_t status = sat_status_set (&status, true, "");

    struct addrinfo *info = NULL;

    for (info = info_list; info != NULL; info = info->ai_next)
    {
        status = sat_udp_client_set_socket (object, info);
        if (sat_status_get_result (&status) == false)
        {
            continue;
        }

        break;
    }

    freeaddrinfo (info_list);

    return status;
}