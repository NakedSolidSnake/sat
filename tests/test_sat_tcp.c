#include <pthread.h>
#include <sat.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE     1024

static bool running = true;

void on_receive (char *buffer, uint32_t *size, void *data)
{
    (void)size;
    (void)data;

    if (strcmp (buffer, "exit") == 0)
        running = false;

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

    sat_tcp_t server;
    char buffer [BUFFER_SIZE] = {0};

    sat_tcp_args_t _args = 
    {
        .type = sat_tcp_type_server,
        .server = 
        {
            .service = "1234",
            .buffer = buffer,
            .size = BUFFER_SIZE,
            .events = 
            {
                .on_receive = on_receive,
                .on_send = on_send
            },
            .type = sat_tcp_server_type_interactive,
        }
    };

    sat_status_t status = sat_tcp_init (&server);
    assert (sat_status_get_result (&status) == true);

    status = sat_tcp_open (&server, &_args);
    assert (sat_status_get_result (&status) == true);


    while (running == true)
    {
        status = sat_tcp_run (&server);
        assert (sat_status_get_result (&status) == true);
    }


    status = sat_tcp_close (&server);
    assert (sat_status_get_result (&status) == true);

    return NULL;
}

static void *client_thread (void *args)
{
    (void)args;

    uint8_t send_count = 0;

    char buffer [BUFFER_SIZE] = {0};
    uint32_t size = BUFFER_SIZE;

    char *hello_message = "Hello World\n";
    char *exit_message = "exit";

    sat_tcp_t client;

    sat_tcp_args_t _args = 
    {
        .type = sat_tcp_type_client,
        .client = 
        {
            .hostname = "localhost",
            .service = "1234"
        }
    };

    sat_status_t status = sat_tcp_init (&client);
    assert (sat_status_get_result (&status) == true);

    status = sat_tcp_open (&client, &_args);
    assert (sat_status_get_result (&status) == true);

    while (send_count++ < 10)
    {
        status = sat_tcp_send (&client, hello_message, strlen (hello_message));
        assert (sat_status_get_result (&status) == true);

        status = sat_tcp_receive (&client, buffer, &size);
        assert (sat_status_get_result (&status) == true);

        assert (strcmp (hello_message, buffer) == 0);
    }

    status = sat_tcp_send (&client, exit_message, strlen (exit_message));
    assert (sat_status_get_result (&status) == true);

    status = sat_tcp_close (&client);
    assert (sat_status_get_result (&status) == true);

    return NULL;
}