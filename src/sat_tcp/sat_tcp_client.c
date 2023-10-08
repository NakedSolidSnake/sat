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

static sat_status_t sat_tcp_client_set_socket (sat_tcp_t *object);
static sat_status_t sat_tcp_client_get_ip_by_hostname (sat_tcp_t *object, sat_tcp_args_t *args);

sat_status_t sat_tcp_client_open (sat_tcp_t *object, sat_tcp_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp client open error");

    if (args->hostname != NULL && args->port != NULL)
    {
        strncpy (object->hostname, args->hostname, SAT_TCP_HOSTNAME_SIZE);
        object->port = args->port;

        do 
        {
            status = sat_tcp_client_get_ip_by_hostname (object, args);
            if (sat_status_get_result (&status) == false)
                break;

            status = sat_tcp_client_set_socket (object);
            
        } while (false);
    }

    sat_status_set (&status, true, "");

    return status;
}

sat_status_t sat_tcp_client_connect (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp client connect error");

    struct sockaddr_in addr_in;

    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons (atoi (object->port));

    if (inet_pton (AF_INET, object->hostname, &addr_in.sin_addr) > 0)
    {
        sat_status_set (&status, false, "sat tcp client connection error");

        if (connect (object->socket, (struct sockaddr *)&addr_in, sizeof (addr_in)) >= 0)
            sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_tcp_client_set_socket (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp client set socket error");

    object->socket = socket (PF_INET, SOCK_STREAM, 0);
    if (object->socket >= 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_tcp_client_get_ip_by_hostname (sat_tcp_t *object, sat_tcp_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp client get ip error");
    struct hostent *he = NULL;

    he = gethostbyname (args->hostname);

    if (he != NULL)
    {
        strncpy (object->hostname, inet_ntoa (*(struct in_addr *)he->h_addr), SAT_TCP_HOSTNAME_SIZE);
        sat_status_set (&status, true, "");
    }

    return status;
}
