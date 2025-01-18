#include <sat_channel.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

sat_status_t sat_channel_init (sat_channel_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_channel_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_channel_open (sat_channel_t *object, sat_channel_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache open error");

    // if (object != NULL && args != NULL && args->buffer != NULL && args->size > 0)
    if (object != NULL)
    {
        if (socketpair (AF_LOCAL, SOCK_STREAM, 0, object->pair) == 0)
        {
            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_channel_write (sat_channel_t *object, uint8_t *buffer, uint32_t size)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache write error");

    if (object != NULL && buffer != 0 && size > 0)
    {
        send (object->pair [0], buffer, size, 0);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_channel_read (sat_channel_t *object, uint8_t *buffer, uint32_t size)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache read error");

    if (object != NULL && buffer != 0 && size > 0)
    {
        recv (object->pair [1], buffer, size, 0);
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_channel_close (sat_channel_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat cache read error");

    if (object != NULL)
    {
        close (object->pair [0]);
        close (object->pair [1]);

        memset (object, 0, sizeof (sat_channel_t));

        sat_status_set (&status, true, "");
    }

    return status;
}
