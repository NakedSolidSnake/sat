#include <sat_udp_server.h>
#include <sat_udp_server_base.h>
#include <sat_udp_server_interactive.h>
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

static sat_status_t sat_udp_server_select_type (sat_udp_server_t *object, sat_udp_server_type_t type);

sat_status_t sat_udp_server_open (sat_udp_server_t **object, sat_udp_server_args_t *args)
{
    sat_status_t status;

    do
    {
        sat_udp_server_t *__object = calloc (1, sizeof (sat_udp_server_t));
        if (__object == NULL)
        {
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

sat_status_t sat_udp_server_run (sat_udp_server_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server run error");

    status = object->base->run (object->base);

    return status;
}

int sat_udp_server_get_socket (sat_udp_server_t *object)
{
    return object->base->get_socket (object->base);
}

static sat_status_t sat_udp_server_select_type (sat_udp_server_t *object, sat_udp_server_type_t type)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server abstract select type error");
    
    if (type == sat_udp_server_type_interactive)
    {

        object->base = sat_udp_server_interactive_create ();

        sat_status_set (&status, true, "");
    }

    return status;
}