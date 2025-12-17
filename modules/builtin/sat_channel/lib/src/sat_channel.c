#include <sat_channel.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

sat_status_t sat_channel_init (sat_channel_t *const object)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            status = sat_status_failure (&status, "sat cache init error: object is null");
            break;
        }

        memset (object, 0, sizeof (sat_channel_t));

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_channel_open (sat_channel_t *const object, const sat_channel_args_t *const args)
{
    sat_status_t status = sat_status_success (&status);
    (void) args;

    do 
    {
        if (object == NULL)
        {
            status = sat_status_failure (&status, "sat cache open error: object is null");
            break;
        }

        if (socketpair (AF_LOCAL, SOCK_STREAM, 0, object->pair) != 0)
        {
            status = sat_status_failure (&status, "sat cache open error: socketpair failed");
            break;
        }
    } while (false);

    return status;
}

sat_status_t sat_channel_write (sat_channel_t *const object, const uint8_t *const buffer, uint32_t size)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            status = sat_status_failure (&status, "sat cache write error: object is null");
            break;
        }

        if (buffer == NULL)
        {
            status = sat_status_failure (&status, "sat cache write error: buffer is null");
            break;
        }

        if (size == 0)
        {
            status = sat_status_failure (&status, "sat cache write error: size is zero");
            break;
        }

        if (send (object->pair [0], buffer, size, 0) < 0)
        {
            status = sat_status_failure (&status, "sat cache write error: send failed");
            break;
        }

    } while (false);

        return status;
}

sat_status_t sat_channel_read (sat_channel_t *const object, uint8_t *const buffer, uint32_t size)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            status = sat_status_failure (&status, "sat cache read error: object is null");
            break;
        }

        if (buffer == NULL)
        {
            status = sat_status_failure (&status, "sat cache read error: buffer is null");
            break;
        }

        if (size == 0)
        {
            status = sat_status_failure (&status, "sat cache read error: size is zero");
            break;
        }

        if (recv (object->pair [1], buffer, size, 0) < 0)
        {
            status = sat_status_failure (&status, "sat cache read error: recv failed");
            break;
        }

    } while (false);

    return status;
}

sat_status_t sat_channel_close (sat_channel_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    do 
    {
        if (object == NULL)
        {
            status = sat_status_failure (&status, "sat cache close error: object is null");
            break;
        }

        close (object->pair [0]);
        close (object->pair [1]);

        memset (object, 0, sizeof (sat_channel_t));

    } while (false);

    return status;
}
