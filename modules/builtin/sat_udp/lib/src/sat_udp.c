#include <sat_udp.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <poll.h>

#include <sat_udp_client.h>
#include <sat_udp_server.h>

static int sat_udp_get_socket (const sat_udp_t *const object);
static void sat_udp_type_destroy (sat_udp_t *const object);

static sat_status_t sat_udp_type_open (sat_udp_t *const object, const sat_udp_args_t *const args);

sat_status_t sat_udp_init (sat_udp_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    do 
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat udp init error: null object");
            break;
        }

        memset (object, 0, sizeof (sat_udp_t));

    } while (false);

        return status;
}

sat_status_t sat_udp_open (sat_udp_t *const object, const sat_udp_args_t *const args)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            status = sat_status_set (&status, false, "sat udp open error: null object");
            break;
        }

        if (args == NULL)
        {
            status = sat_status_set (&status, false, "sat udp open error: null args");
            break;
        }

        status = sat_udp_type_open (object, args);

    } while (false);

    return status;
}

sat_status_t sat_udp_run (sat_udp_t *const object)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            status = sat_status_set (&status, false, "sat udp run error: null object");
            break;
        }

        if (object->type != sat_udp_type_server)
        {
            status = sat_status_set (&status, false, "sat udp run error: invalid type");
            break;
        }

        status = sat_udp_server_run (object->server);

    } while (false);

        return status;
}

sat_status_t sat_udp_send (const sat_udp_t *const object, const char *const data, uint32_t size, const sat_udp_destination_t *const destination)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            status = sat_status_set (&status, false, "sat udp send error: null object");
            break;
        }

        if (data == NULL)
        {
            status = sat_status_set (&status, false, "sat udp send error: null data or zero size");
            break;
        }

        if (size == 0)
        {
            status = sat_status_set (&status, false, "sat udp send error: null data or zero size");
            break;
        }

        if (destination == NULL)
        {
            status = sat_status_set (&status, false, "sat udp send error: null destination");
            break;
        }

        if (destination->hostname == NULL)
        {
            status = sat_status_set (&status, false, "sat udp send error: null destination hostname");
            break;
        }

        if ( destination->service == NULL)
        {
            status = sat_status_set (&status, false, "sat udp send error: null destination service");
            break;
        }

        struct addrinfo hints;
        memset (&hints, 0, sizeof (hints));
        hints.ai_family = AF_UNSPEC;

        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;

        struct addrinfo *address;
        size_t _size;

        int r = getaddrinfo (destination->hostname, destination->service, &hints, &address);

        if (r != 0)
        {
            status = sat_status_set (&status, false, "sat udp send error: getaddrinfo failed");
            break;
        }

        int socket = sat_udp_get_socket (object);

        _size = sendto (socket, data, size, 0, address->ai_addr, address->ai_addrlen);
        if (_size != size)
        {
            status = sat_status_set (&status, false, "sat udp send error: sendto failed");
        }

        freeaddrinfo (address);

    } while (false);

    return status;
}

sat_status_t sat_udp_receive (const sat_udp_t *const object, char *const data, uint32_t *size, int timeout_ms)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp receive error");

    struct sockaddr_storage source;
    socklen_t source_len = sizeof (source);
    size_t _size = 0;

    if (object != NULL && data != NULL && size != NULL && *size > 0)
    {
        int socket = sat_udp_get_socket (object);

        struct pollfd pfd;
        pfd.fd = socket;
        pfd.events = POLLIN;
        pfd.revents = 0;

        int poll_result = poll (&pfd, 1, timeout_ms);

        if (poll_result > 0 && (pfd.revents & POLLIN))
        {
            // Data available, receive it
            memset (data, 0, *size);
            _size = recvfrom (socket, data, *size, 0, (struct sockaddr *)&source, &source_len);
            
            if (_size > 0)
            {
                data [_size] = 0;
                *size = _size;
                sat_status_set (&status, true, "");
            }
        }
        else if (poll_result == 0)
        {
            // Timeout occurred
            sat_status_set (&status, false, "sat udp receive timeout");
        }
        else
        {
            // Error occurred
            sat_status_set (&status, false, "sat udp receive poll error");
        }

        // memset (data, 0, *size);

        // _size = recvfrom (socket, data, *size, 0, (struct sockaddr *)&source, &source_len);
        // data [_size] = 0;

        // sat_status_set (&status, true, "");
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

sat_status_t sat_udp_get_port (const sat_udp_t *const object, uint16_t *const port)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp get port error");

    if (object != NULL && port != NULL)
    {
        if (object->type == sat_udp_type_server)
        {
            status = sat_udp_server_get_port (object->server, port);
        }
        
        else if (object->type == sat_udp_type_client)
        {
            status = sat_udp_client_get_port (object->client, port);
        }
    }

    return status;
}

static int sat_udp_get_socket (const sat_udp_t *const object)
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

static sat_status_t sat_udp_type_open (sat_udp_t *const object, const sat_udp_args_t *const args)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp type open error");

    object->type = args->type;

    if (object->type == sat_udp_type_server)
    {
        status = sat_udp_server_open (&object->server, &args->server);
    }
    
    else if (object->type == sat_udp_type_client)
    {
        status = sat_udp_client_open (&object->client, &args->client);
    }

    return status;
}