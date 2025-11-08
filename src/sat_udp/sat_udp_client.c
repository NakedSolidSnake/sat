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

static sat_status_t sat_udp_client_set_socket (sat_udp_client_t *object, struct addrinfo *info);


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

sat_status_t sat_udp_client_get_port (sat_udp_client_t *object, uint16_t *port)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp client get port error");

    do 
    {
        if (object == NULL || port == NULL)
        {
            sat_status_set (&status, false, "Invalid parameters");
            break;
        }

        struct sockaddr_storage address;
        socklen_t address_len = sizeof (address);

        if (getsockname (object->socket, (struct sockaddr*)&address, &address_len) < 0)
        {
            sat_status_set (&status, false, "Failed to get socket name");
            break;
        }

        if (address.ss_family == AF_INET)
        {
            struct sockaddr_in *addr4 = (struct sockaddr_in*)&address;
            *port = ntohs (addr4->sin_port);
        }
        else if (address.ss_family == AF_INET6)
        {
            struct sockaddr_in6 *addr6 = (struct sockaddr_in6*)&address;
            *port = ntohs (addr6->sin6_port);
        }
        else
        {
            sat_status_set (&status, false, "Unknown address family");
            break;
        }

        sat_status_set (&status, true, "");

    } while (false);

    return status;
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
