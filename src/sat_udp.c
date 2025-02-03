#include <sat_udp.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sat_udp_client.h>
#include <sat_udp_server.h>

static int sat_udp_get_socket (sat_udp_t *object);
static void sat_udp_type_destroy (sat_udp_t *object);

static sat_status_t sat_udp_type_open (sat_udp_t *object, sat_udp_args_t *args);

sat_status_t sat_udp_init (sat_udp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_udp_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_udp_open (sat_udp_t *object, sat_udp_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp open error");

    if (object != NULL && args != NULL)
    {
        status = sat_udp_type_open (object, args);
    }

    return status;
}

sat_status_t sat_udp_run (sat_udp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp run error");

    if (object != NULL && object->type == sat_udp_type_server)
    {
        while (true)
        {
            sat_udp_server_run (object->server);
        }

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_udp_send (sat_udp_t *object, const char *data, uint32_t size, sat_udp_destination_t *destination)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp send error");

    size_t _size;

    if (object != NULL && data != NULL && size > 0)
    {
        struct addrinfo hints;
        memset (&hints, 0, sizeof (hints));
        hints.ai_family = AF_UNSPEC;

        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;

        struct addrinfo *address;

        int r = getaddrinfo (destination->hostname, destination->service, &hints, &address);

        if (r == 0)
        {
            int socket = sat_udp_get_socket (object);

            _size = sendto (socket, data, size, 0, address->ai_addr, address->ai_addrlen);
            _size == size ?  sat_status_set (&status, true, "") :  sat_status_set (&status, false, "sat udp send size error");

            freeaddrinfo (address);
        }
    }

    return status;
}

sat_status_t sat_udp_receive (sat_udp_t *object, char *data, uint32_t *size)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp receive error");

    struct sockaddr_storage source;
    socklen_t source_len = sizeof (source);
    size_t _size = 0;

    if (object != NULL && data != NULL && size != NULL && *size > 0)
    {
        int socket = sat_udp_get_socket (object);

        memset (data, 0, *size);

        _size = recvfrom (socket, data, *size, 0, (struct sockaddr *)&source, &source_len);
        data [_size] = 0;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_udp_close (sat_udp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp init error");

    if (object != NULL)
    {
        int socket = sat_udp_get_socket (object);
        
        close (socket);

        sat_udp_type_destroy (object);

        memset (object, 0, sizeof (sat_udp_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

static int sat_udp_get_socket (sat_udp_t *object)
{
    int socket = -1;

    if (object->type == sat_udp_type_server)
        socket = sat_udp_server_get_socket (object->server);
    else 
        socket = sat_udp_client_get_socket (object->client);

    return socket;
}

static void sat_udp_type_destroy (sat_udp_t *object)
{
    if (object->type == sat_udp_type_server)
        free (object->server);
    else 
        free (object->client);
}

static sat_status_t sat_udp_type_open (sat_udp_t *object, sat_udp_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp type open error");

    object->type = args->type;

    if (object->type == sat_udp_type_server)
        status = sat_udp_server_open (&object->server, &args->server);
    
    else if (object->type == sat_udp_type_client)
        status = sat_udp_client_open (&object->client, &args->client);

    return status;
}