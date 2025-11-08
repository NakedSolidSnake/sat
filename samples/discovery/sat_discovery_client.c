#include <pthread.h>
#include <sat.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct 
{
    sat_udp_t client;
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
            .name = "Client",
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

    status = sat_discovery_add_interest (&app->discovery, "Server");
    if (sat_status_get_result (&status) == false)
        return status;

    status = sat_discovery_start (&app->discovery);
    if (sat_status_get_result (&status) == false)
        return status;

    return sat_status_set (&status, true, "discovery started");
}


int main (int argc, char *argv[])
{
    char buffer [BUFFER_SIZE] = {0};
    uint32_t size = BUFFER_SIZE;
    const char *const message = "Hello from SAT Discovery Client!";

    sat_log_init_by_env ("");

    application_t app;
    sat_discovery_service_info_t info;

    sat_udp_args_t _args = 
    {
        .type = sat_udp_type_client,
    };

    sat_status_t status = sat_udp_init (&app.client);
    assert (sat_status_get_result (&status) == true);

    status = sat_udp_open (&app.client, &_args);
    assert (sat_status_get_result (&status) == true);

    discovery_start (&app);

    while (true)
    {
        sleep (1);

        sat_discovery_registered_services (&app.discovery);

        status = sat_discovery_get_service_info (&app.discovery, "Server", &info);
        if (sat_status_get_result (&status) == true)
        {

            sat_log_info ("Sending message to service %s at %s:%s", info.name, info.address, info.port);
            status = sat_udp_send (&app.client, message, strlen (message), &(sat_udp_destination_t)
                                                                {
                                                                    .hostname = info.address,
                                                                    .service = info.port
                                                                });

            // status = sat_udp_receive (&app.client, buffer, &size);
            // assert (sat_status_get_result (&status) == true);
            // printf ("%s\n", buffer);
        }
    }
    
    status = sat_udp_close (&app.client);
    assert (sat_status_get_result (&status) == true);

    return 0;
}

