#include <sat_channel.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

sat_status_t sat_channel_init (sat_channel_t *const object)
{
    sat_status_return_on_null (object, "object is null");

    memset (object, 0, sizeof (sat_channel_t));

    sat_status_return_on_success ();
}

sat_status_t sat_channel_open (sat_channel_t *const object, const sat_channel_args_t *const args)
{
    sat_status_return_on_null (object, "object is null");
    sat_status_return_on_not_equals (socketpair (AF_LOCAL, SOCK_STREAM, 0, object->pair), 0, "socketpair failed");
   
    sat_status_return_on_success ();
}

sat_status_t sat_channel_write (sat_channel_t *const object, const uint8_t *const buffer, uint32_t size)
{
    sat_status_return_on_null (object, "object is null");
    sat_status_return_on_null (buffer, "buffer is null");
    sat_status_return_on_equals (size, 0, "size is zero");

    sat_status_return_on_less_than (send (object->pair [0], buffer, size, 0), 0, "send failed");

    sat_status_return_on_success ();
}

sat_status_t sat_channel_read (sat_channel_t *const object, uint8_t *const buffer, uint32_t size)
{
    sat_status_return_on_null (object, "object is null");
    sat_status_return_on_null (buffer, "buffer is null");
    sat_status_return_on_equals (size, 0, "size is zero");

    sat_status_return_on_less_than (recv (object->pair [1], buffer, size, 0), 0, "recv failed");

    sat_status_return_on_success ();
}

sat_status_t sat_channel_close (sat_channel_t *const object)
{
    sat_status_return_on_null (object, "object is null");

    close (object->pair [0]);
    close (object->pair [1]);

    memset (object, 0, sizeof (sat_channel_t));

    sat_status_return_on_success ();
}
