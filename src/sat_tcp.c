#include <sat_tcp.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sat_tcp_client.h>
#include <sat_tcp_server.h>

typedef sat_status_t (*sat_tcp_open_t) (sat_tcp_t *object, sat_tcp_args_t *args);

static sat_status_t sat_tcp_type_open (sat_tcp_t *object, sat_tcp_args_t *args);
static sat_status_t sat_tcp_type_error_open (sat_tcp_t *object, sat_tcp_args_t *args);

sat_status_t sat_tcp_init (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_tcp_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_tcp_open (sat_tcp_t *object, sat_tcp_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp open error");

    if (object != NULL && args != NULL)
    {
        status = sat_tcp_type_open (object, args);
    }

    return status;
}

sat_status_t sat_tcp_run (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp run error");

    if (object != NULL && object->type == sat_tcp_type_server)
    {
        status = sat_tcp_server_run (object);
    }

    return status;
}

sat_status_t sat_tcp_connect (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp connect error");

    if (object != NULL&& object->type == sat_tcp_type_client)
    {
        status = sat_tcp_client_connect (object);
    }

    return status;
}

sat_status_t sat_tcp_send (sat_tcp_t *object, const char *data, uint32_t size)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp send error");
    uint32_t _size;

    if (object != NULL && data != NULL && size > 0)
    {
        _size = send (object->socket, data, size, 0);

        if (_size == size)
            sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_tcp_receive (sat_tcp_t *object, char *data, uint32_t *size)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp receive error");

    size_t _size = 0;

    if (object != NULL && data != NULL && size != NULL && *size > 0)
    {
        _size = recv (object->socket, data, *size, 0);

        *size = _size;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_tcp_close (sat_tcp_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp close error");

    if (object != NULL)
    {
        shutdown (object->socket, SHUT_RDWR);

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_tcp_type_open (sat_tcp_t *object, sat_tcp_args_t *args)
{
    sat_tcp_open_t open = sat_tcp_type_error_open;

    object->type = args->type;

    if (object->type == sat_tcp_type_server)
        open = sat_tcp_server_open;
    
    else if (object->type == sat_tcp_type_client)
        open = sat_tcp_client_open;

    return open (object, args);
}

static sat_status_t sat_tcp_type_error_open (sat_tcp_t *object, sat_tcp_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp type error");

    (void) object;
    (void) args;

    return status;
}