#include <sat.h>
#include <stdio.h>


int main (int argc, char *argv[])
{
    if (argc < 4)
    {
        fprintf (stderr, "Usage: %s <service_name> <address> <port>\n", argv [0]);
        return EXIT_FAILURE;
    }

    sat_log_init_by_env ("");

    sat_log_info ("Starting SAT Discovery Sample");
    
    sat_discovery_t discovery;
    sat_discovery_args_t args = 
    {
        .service = 
        {
            .name = argv [1],
            .interface = NULL,
            .port = "1234"
        },

        .channel = 
        {
            .service = argv [3],
            .address = argv [2],
        }
    };

    sat_status_t status = sat_discovery_init (&discovery);
    if (sat_status_get_result (&status) == false)
    {
        sat_log_error ("Failed to initialize discovery: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }

    status = sat_discovery_open (&discovery, &args);
    if (sat_status_get_result (&status) == false)
    {
        sat_log_error ("Failed to open discovery: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }

    for (int i = 4; i < argc; i++)
    {
        char interest_name [64];
        snprintf (interest_name, sizeof (interest_name), "%s", argv [i]);

        sat_log_info ("Adding interest: %s", interest_name);

        status = sat_discovery_add_interest (&discovery, interest_name);
        if (sat_status_get_result (&status) == false)
        {
            sat_log_error ("Failed to add interest '%s': %s\n", interest_name, sat_status_get_motive (&status));
            return EXIT_FAILURE;
        }
    }

    sat_log_info ("Starting SAT Discovery Service '%s' on channel '%s' at address '%s'", 
                  args.service.name, args.channel.service, args.channel.address);

    status = sat_discovery_start (&discovery);
    if (sat_status_get_result (&status) == false)
    {
        sat_log_error ("Failed to start discovery: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }

    // Cleanup
    sat_log_debug ("Discovery service running. Press Ctrl+C to stop.");

    sleep (300); // Let the discovery run for 30 seconds
    sat_discovery_stop (&discovery);

    return EXIT_SUCCESS;
}