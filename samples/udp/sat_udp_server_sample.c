#include <sat.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVICE_NAME    "8888"

#define BUFFER_SIZE     1024

void on_receive (char *buffer, uint32_t *size, void *data)
{
    (void)size;
    (void)data;
    if (strcmp (buffer, "exit") == 0)
        exit (0);

    printf ("Server: %s", buffer);
}

void on_send (char *buffer, uint32_t *size, void *data)
{
    (void)data;

    char *hello_message = "Hi, from server";
    strncpy (buffer, hello_message, strlen (hello_message));
    *size = strlen (hello_message);
}


int main (int argc, char *argv[])
{
    sat_udp_t server;
    char buffer [BUFFER_SIZE] = {0};

    sat_udp_server_type_t type = sat_udp_server_type_interactive;

    if (argc < 2 || argc > 3)
    {
        fprintf (stderr, "Usage: %s <port> <mode> \n", argv [0]);
        fprintf (stderr, "mode: interactive [default]\n");
        fprintf (stderr, "      async\n");
        return EXIT_FAILURE;
    }

    if (argc == 3 && strcmp (argv [2], "async") == 0)
    {
        type = sat_udp_server_type_async;
    }

    sat_udp_args_t _args = 
    {
        .type = sat_udp_type_server,
        .server = 
        {
            .service = argv [1],
            .buffer = buffer,
            .size = BUFFER_SIZE,
            .events = 
            {
                .on_receive = on_receive,
                .on_send = on_send
            },
            .type = type
        }
        
    };

    printf ("Waiting messages on port: %s\n", argv [1]);

    sat_status_t status = sat_udp_init (&server);
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_open (&server, &_args);
    assert (sat_status_get_result (&status) == true);

    do
    {
        status = sat_udp_run (&server);
    } while (sat_status_get_result (&status) == true);

    status = sat_udp_close (&server);
    assert (sat_status_get_result (&status) == true);

    return 0;
}
