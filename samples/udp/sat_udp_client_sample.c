#include <pthread.h>
#include <sat.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define BUFFER_SIZE     1024


int main (int argc, char *argv[])
{
    char buffer [BUFFER_SIZE] = {0};
    uint32_t size = BUFFER_SIZE;

    sat_udp_t client;

    sat_udp_args_t _args = 
    {
        .type = sat_udp_type_client,
    };

    if (argc != 4)
    {
        fprintf (stdout, "Usage: %s <hostname> <port> <message>\n", argv [0]);
        return EXIT_FAILURE;
    }

    sat_status_t status = sat_udp_init (&client);
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_open (&client, &_args);
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_send (&client, argv [3], strlen (argv [3]), &(sat_udp_destination_t)
                                                                {
                                                                    .hostname = argv [1],
                                                                    .service = argv [2]
                                                                });
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_receive (&client, buffer, &size);
    assert (sat_status_get_result (&status) == true);

    printf ("%s\n", buffer);

    status = sat_udp_close (&client);
    assert (sat_status_get_result (&status) == true);

    return 0;
}

