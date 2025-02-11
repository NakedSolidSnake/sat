#include <sat_udp_server_interactive.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>

static sat_status_t sat_udp_server_interactive_open (void *object, sat_udp_server_args_t *args);
static sat_status_t sat_udp_server_interactive_run (void *object);
static int sat_udp_server_interactive_get_socket (void *object);

sat_udp_server_base_t *sat_udp_server_interactive_create (void)
{
    static sat_udp_server_interactive_t interactive =
    {
        .abstract.base =
        {
            .object = &interactive,
            .open = sat_udp_server_interactive_open,
            .run = sat_udp_server_interactive_run,
            .get_socket = sat_udp_server_interactive_get_socket
        } 
    };

    return (sat_udp_server_base_t *)&interactive;
}

static sat_status_t sat_udp_server_interactive_run (void *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server interactive error");

    sat_udp_server_interactive_t *interactive = (sat_udp_server_interactive_t *) object;

    struct sockaddr_storage source;
    socklen_t length = sizeof (source);
    uint32_t size;

    memset (&source, 0, sizeof (struct sockaddr_storage));

    size = recvfrom (interactive->abstract.socket,
                     interactive->abstract.buffer,
                     interactive->abstract.size,
                     0,
                     (struct sockaddr *)&source,
                     &length);

    interactive->abstract.buffer [size] = 0;

    if (interactive->abstract.events.on_receive)
        interactive->abstract.events.on_receive (interactive->abstract.buffer, &size, interactive->abstract.data);

    if (interactive->abstract.events.on_send)
    {
        interactive->abstract.events.on_send (interactive->abstract.buffer, &size, interactive->abstract.data);
        size = size > interactive->abstract.size ? interactive->abstract.size : size;
        sendto (interactive->abstract.socket,
                interactive->abstract.buffer,
                size,
                0,
                (struct sockaddr *)&source,
                sizeof (source));                    
    }

    memset (interactive->abstract.buffer, 0, interactive->abstract.size);

    sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_udp_server_interactive_open (void *object, sat_udp_server_args_t *args)
{
    sat_udp_server_interactive_t *interactive = (sat_udp_server_interactive_t *) object;

    return sat_udp_server_abstract_open (&interactive->abstract, args);
}

static int sat_udp_server_interactive_get_socket (void *object)
{
    sat_udp_server_interactive_t *interactive = (sat_udp_server_interactive_t *) object;

    return sat_udp_server_abstract_get_socket (&interactive->abstract);
}