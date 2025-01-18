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

typedef sat_status_t (*sat_udp_open_t) (sat_udp_t *object, sat_udp_args_t *args);

static sat_status_t sat_udp_type_open (sat_udp_t *object, sat_udp_args_t *args);
static sat_status_t sat_udp_type_error_open (sat_udp_t *object, sat_udp_args_t *args);

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
            sat_udp_server_run (object);
        }

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_udp_send (sat_udp_t *object, const char *data, uint32_t size)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp send error");

    // struct sockaddr_in address_in;
    size_t _size;

    if (object != NULL && data != NULL && size > 0)
    {
        // memset (&address_in, 0, sizeof (struct sockaddr_in));

        struct addrinfo hints;
        memset (&hints, 0, sizeof (hints));
        hints.ai_family = AF_UNSPEC;

        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;

        struct addrinfo *address;

        int r = getaddrinfo (object->hostname, object->service, &hints, &address);

        if (r == 0)
        {
            _size = sendto (object->socket, data, size, 0, address->ai_addr, address->ai_addrlen);
            _size == size ?  sat_status_set (&status, true, "") :  sat_status_set (&status, false, "sat udp send size error");

            freeaddrinfo (address);
        }

        // address_in.sin_family = AF_INET;
        // address_in.sin_addr.s_addr = inet_addr (object->hostname);
        // address_in.sin_port = htons (atoi (object->service));
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
        memset (data, 0, *size);
        _size = recvfrom (object->socket, data, *size, 0, (struct sockaddr *)&source, &source_len);
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
        close (object->socket);

        memset (object, 0, sizeof (sat_udp_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_udp_type_open (sat_udp_t *object, sat_udp_args_t *args)
{
    sat_udp_open_t open = sat_udp_type_error_open;

    object->type = args->type;

    if (object->type == sat_udp_type_server)
        open = sat_udp_server_open;
    
    else if (object->type == sat_udp_type_client)
        open = sat_udp_client_open;

    return open (object, args);
}

static sat_status_t sat_udp_type_error_open (sat_udp_t *object, sat_udp_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp type error");

    (void) object;
    (void) args;

    return status;
}