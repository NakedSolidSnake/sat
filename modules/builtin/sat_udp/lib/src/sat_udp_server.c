#include <sat_udp_server.h>
#include <sat_udp_server_base.h>
#include <sat_udp_server_interactive.h>
#include <sat_udp_server_async.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>


struct sat_udp_server_t
{
    sat_udp_server_base_t *base;
};

static sat_status_t sat_udp_server_select_type (sat_udp_server_t *const object, sat_udp_server_type_t type);

sat_status_t sat_udp_server_open (sat_udp_server_t **const object, const sat_udp_server_args_t *const args)
{
    sat_status_t status;

    do
    {
        sat_udp_server_t *__object = calloc (1, sizeof (sat_udp_server_t));
        if (__object == NULL)
        {
            status = sat_status_failure (&status, "sat udp server open error: memory allocation failed");
            break;
        }

        status = sat_udp_server_select_type (__object, args->type);
        if (sat_status_get_result (&status) == false)
        {
            free (__object);
            break;
        }

        status = __object->base->open (__object->base, args);
        if (sat_status_get_result (&status) == false)
        {
            free (__object);
            break;
        }

        *object = __object;

    } while (false);

    return status;
}

sat_status_t sat_udp_server_run (sat_udp_server_t *const object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server run error");

    status = object->base->run (object->base);

    return status;
}

int sat_udp_server_get_socket (sat_udp_server_t *object)
{
    return object->base->get_socket (object->base);
}

sat_status_t sat_udp_server_get_port (sat_udp_server_t *object, uint16_t *port)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server get port error");

    if (object != NULL && port != NULL)
    {
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof (addr);

        int socket = sat_udp_server_get_socket (object);
        
        if (getsockname (socket, (struct sockaddr *)&addr, &addr_len) == 0)
        {
            *port = ntohs (addr.sin_port);
            sat_status_success (&status);
        }
    }

    return status;
}

static sat_status_t sat_udp_server_select_type (sat_udp_server_t *object, sat_udp_server_type_t type)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server select type error");
    
    if (type == sat_udp_server_type_interactive)
    {
        object->base = sat_udp_server_interactive_create ();

        sat_status_set (&status, true, "");
    }

    else if (type == sat_udp_server_type_async)
    {
        object->base = sat_udp_server_async_create ();

        sat_status_set (&status, true, "");
    }

    return status;
}