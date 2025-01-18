#include <sat_udp_client.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

static sat_status_t sat_udp_client_set_socket (sat_udp_t *object);
static sat_status_t sat_udp_client_get_ip_by_hostname (sat_udp_t *object, sat_udp_args_t *args);

sat_status_t sat_udp_client_open (sat_udp_t *object, sat_udp_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp client open error");

    if (args->hostname != NULL && args->service != NULL)
    {
        strncpy (object->hostname, args->hostname, SAT_UDP_HOSTNAME_SIZE);
        object->service = args->service;

        do 
        {
            status = sat_udp_client_get_ip_by_hostname (object, args);
            if (sat_status_get_result (&status) == false)
                break;

            status = sat_udp_client_set_socket (object);
            
        } while (false);
    }

    sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_udp_client_set_socket (sat_udp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp client set socket error");

    object->socket = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (object->socket >= 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_udp_client_get_ip_by_hostname (sat_udp_t *object, sat_udp_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp client get ip error");
    struct hostent *he = NULL;

    he = gethostbyname (args->hostname);

    if (he != NULL)
    {
        strncpy (object->hostname, inet_ntoa (*(struct in_addr *)he->h_addr), SAT_UDP_HOSTNAME_SIZE);
        sat_status_set (&status, true, "");
    }

    return status;
}
