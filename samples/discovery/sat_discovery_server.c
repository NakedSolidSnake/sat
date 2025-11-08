#include <sat.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct 
{
    sat_udp_t udp;
    sat_discovery_t discovery;
    char port [6];
} application_t;

#define BUFFER_SIZE     1024


static sat_status_t discovery_start (application_t *app)
{
    sat_status_t status = sat_status_set (&status, false, "discovery start error");

    sat_discovery_args_t args = 
    {
        .service = 
        {
            .name = "Server",
            .interface = NULL,
            .port = app->port
        },
        .channel = 
        {
            .service = "1234",
            .address = "234.1.1.1"
        }
    };

    status = sat_discovery_open (&app->discovery, &args);
    if (sat_status_get_result (&status) == false)
        return status;

    status = sat_discovery_start (&app->discovery);
    if (sat_status_get_result (&status) == false)
        return status;

    return sat_status_set (&status, true, "discovery started");
}

void on_receive (char *buffer, uint32_t *size, void *data)
{
    (void)size;
    (void)data;
    if (strcmp (buffer, "exit") == 0)
        exit (0);

    printf ("Server: %s\n", buffer);
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
    application_t app;
    char buffer [BUFFER_SIZE] = {0};

    sat_log_init_by_env ("error");

    sat_udp_args_t _args = 
    {
        .type = sat_udp_type_server,
        .server = 
        {
            .service = "",      // It will select automatically an available port
            .buffer = buffer,
            .size = BUFFER_SIZE,
            .events = 
            {
                .on_receive = on_receive,
                .on_send = on_send
            },
            .type = sat_udp_server_type_interactive
        }
    };

    sat_status_t status = sat_udp_init (&app.udp);
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_open (&app.udp, &_args);
    assert (sat_status_get_result (&status) == true);

    uint16_t port;
    status = sat_udp_get_port (&app.udp, &port);
    assert (sat_status_get_result (&status) == true);

    snprintf (app.port, sizeof (app.port), "%u", port);

    discovery_start (&app);

    do
    {
        status = sat_udp_run (&app.udp);
    } while (sat_status_get_result (&status) == true);

    status = sat_udp_close (&app.udp);
    assert (sat_status_get_result (&status) == true);

    return 0;
}
