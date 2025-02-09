#include <sat_udp_server_async.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>

static sat_status_t sat_udp_server_async_open (void *object, sat_udp_server_args_t *args);
static sat_status_t sat_udp_server_async_run (void *object);
static int sat_udp_server_async_get_socket (void *object);

sat_udp_server_base_t *sat_udp_server_async_create (void)
{
    static sat_udp_server_async_t async =
    {
        .base =
        {
            .object = &async,
            .open = sat_udp_server_async_open,
            .run = sat_udp_server_async_run,
            .get_socket = sat_udp_server_async_get_socket
        } 
    };

    return (sat_udp_server_base_t *)&async;
}

static sat_status_t sat_udp_server_async_run (void *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server async error");

    sat_udp_server_async_t *async = (sat_udp_server_async_t *) object;

    async->read = async->master;

    struct timeval timeout = 
    {
        .tv_sec = 0,
        .tv_usec = 100000
    };

    int __status = select (async->max_socket + 1, &async->read, 0, 0, &timeout);

    if (__status < 0)
    {
        // failed
        sat_status_set (&status, false, "Select returns a error");
    }
    else if (__status == 0)
    {
        //timeout
        sat_status_set (&status, true, "");
    }

    if (FD_ISSET (async->abstract.socket, &async->read))
    {
        struct sockaddr_storage source;
        socklen_t length = sizeof (source);
        uint32_t size;

        size = recvfrom (async->abstract.socket,
                        async->abstract.buffer,
                        async->abstract.size,
                        0,
                        (struct sockaddr *)&source,
                        &length);

        async->abstract.buffer [size] = 0;

        if (async->abstract.events.on_receive)
            async->abstract.events.on_receive (async->abstract.buffer, &size, async->abstract.data);

        if (async->abstract.events.on_send)
        {
            async->abstract.events.on_send (async->abstract.buffer, &size, async->abstract.data);
            size = size > async->abstract.size ? async->abstract.size : size;
            sendto (async->abstract.socket,
                    async->abstract.buffer,
                    size,
                    0,
                    (struct sockaddr *)&source,
                    sizeof (source));                    
        }

        memset (async->abstract.buffer, 0, async->abstract.size);

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_udp_server_async_open (void *object, sat_udp_server_args_t *args)
{
    sat_udp_server_async_t *async = (sat_udp_server_async_t *) object;

    sat_status_t status = sat_udp_server_abstract_open (&async->abstract, args);

    if (sat_status_get_result (&status) == true)
    {
        FD_ZERO (&async->master);
        FD_SET (async->abstract.socket, &async->master);

        async->max_socket = async->abstract.socket;
    }

    return status;
}

static int sat_udp_server_async_get_socket (void *object)
{
    sat_udp_server_async_t *async = (sat_udp_server_async_t *) object;

    return sat_udp_server_abstract_get_socket (&async->abstract);
}