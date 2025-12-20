#include <sat_tcp_server_interactive.h>
#include <sat_tcp_server_abstract.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>

static sat_status_t sat_tcp_server_interactive_handle_client (void *object, int client);

sat_tcp_server_base_t sat_tcp_server_interactive_create (void)
{
    sat_tcp_server_base_t base = 
    {
        .handle_client = sat_tcp_server_interactive_handle_client,
    };

    return base;
}

static sat_status_t sat_tcp_server_interactive_handle_client (void *object, int client)
{
    sat_status_t status = sat_status_set (&status, false, "sat tcp server handle client error");
    
    sat_tcp_server_abstract_t *abstract = (sat_tcp_server_abstract_t *) object;

    uint32_t size;

    if (client > 0)
    {
        while (true)
        {
            memset (abstract->buffer, 0, abstract->size);

            size = recv (client, abstract->buffer, abstract->size, 0);

            if (size == 0)
                break;

            else if (abstract->events.on_receive != NULL && size > 0)
            {
                abstract->events.on_receive (abstract->buffer, &size, abstract->data);

                if (abstract->events.on_send != NULL)
                {
                    abstract->events.on_send (abstract->buffer, &size, abstract->data);
                    send (client, abstract->buffer, (int)fmin (size, abstract->size), 0);
                }
            }
        }

        sat_status_set (&status, true, "");
    }
    
    return status;
}