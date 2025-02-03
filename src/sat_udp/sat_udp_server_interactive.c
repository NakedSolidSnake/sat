#include <sat_udp_server_interactive.h>
#include <sat_udp_server_abstract.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>

static sat_status_t sat_udp_server_interactive_run (void *object);

sat_udp_server_base_t sat_udp_server_interactive_create (void)
{
    sat_udp_server_base_t base = 
    {
        .run = sat_udp_server_interactive_run,
    };

    return base;
}

static sat_status_t sat_udp_server_interactive_run (void *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server interactive error");

    sat_udp_server_abstract_t *abstract = (sat_udp_server_abstract_t *) object;

    struct sockaddr_storage source;
    socklen_t length = sizeof (source);
    uint32_t size;

    memset (&source, 0, sizeof (struct sockaddr_storage));

    // if (abstract->socket >= 0)
    // {
    size = recvfrom (abstract->socket,
                        abstract->buffer,
                        abstract->size,
                        0,
                        (struct sockaddr *)&source,
                        &length);

    abstract->buffer [size] = 0;

    if (abstract->events.on_receive)
        abstract->events.on_receive (abstract->buffer, &size, abstract->data);

    if (abstract->events.on_send)
    {
        abstract->events.on_send (abstract->buffer, &size, abstract->data);
        size = size > abstract->size ? abstract->size : size;
        sendto (abstract->socket,
                abstract->buffer,
                size,
                0,
                (struct sockaddr *)&source,
                sizeof (source));                    
    }

    memset (abstract->buffer, 0, abstract->size);

    sat_status_set (&status, true, "");
    // }

    return status;
}