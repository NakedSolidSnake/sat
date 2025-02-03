#include <pthread.h>
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

    char *hello_message = "Hello World\n";
    strncpy (buffer, hello_message, strlen (hello_message));
    *size = strlen (hello_message);
}

static void *server_thread (void *args);
static void *client_thread (void *args);

int main (int argc, char *argv[])
{
    pthread_t server;
    pthread_t client;

    pthread_create (&server, NULL, server_thread, NULL);

    usleep (5000);

    pthread_create (&client, NULL, client_thread, NULL);

    pthread_join (server, NULL);
    pthread_join (client, NULL);

    return 0;
}

static void *server_thread (void *args)
{
    (void)args;

    sat_udp_t server;
    char buffer [BUFFER_SIZE] = {0};

    sat_udp_args_t _args = 
    {
        .type = sat_udp_type_server,
        .server = 
        {
            .service = SERVICE_NAME,
            .buffer = buffer,
            .size = BUFFER_SIZE,
            .events = 
            {
                .on_receive = on_receive,
                .on_send = on_send
            }
        }
        
    };

    sat_status_t status = sat_udp_init (&server);
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_open (&server, &_args);
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_run (&server);
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_close (&server);
    assert (sat_status_get_result (&status) == true);

    return NULL;
}

static void *client_thread (void *args)
{
    (void)args;

    char buffer [BUFFER_SIZE] = {0};
    uint32_t size = BUFFER_SIZE;

    char *hello_message = "Hello World\n";
    char *exit_message = "exit";

    sat_udp_t client;

    sat_udp_args_t _args = 
    {
        .type = sat_udp_type_client,
    };

    sat_status_t status = sat_udp_init (&client);
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_open (&client, &_args);
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_send (&client, hello_message, strlen (hello_message), &(sat_udp_destination_t)
                                                                          {
                                                                            .hostname = "localhost",
                                                                            .service = SERVICE_NAME
                                                                          });
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_receive (&client, buffer, &size);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (hello_message, buffer) == 0);

    status = sat_udp_send (&client, exit_message, strlen (exit_message), &(sat_udp_destination_t)
                                                                          {
                                                                            .hostname = "localhost",
                                                                            .service = SERVICE_NAME
                                                                          });
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_close (&client);
    assert (sat_status_get_result (&status) == true);

    return NULL;
}