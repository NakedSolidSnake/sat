#include <sat.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf (stderr, "Usage: %s <service_name> <address> <port>\n", argv [0]);
        return EXIT_FAILURE;
    }
    
    sat_discovery_t discovery;
    sat_discovery_args_t args = 
    {
        .service.name = argv [1],
        .channel = 
        {
            .service = argv [3],
            .address = argv [2],
        }
    };

    sat_status_t status = sat_discovery_init (&discovery);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to initialize discovery: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }

    status = sat_discovery_open (&discovery, &args);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to open discovery: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }

    for (int i = 4; i < argc; i++)
    {
        char interest_name [64];
        snprintf (interest_name, sizeof (interest_name), "%s", argv [i]);

        status = sat_discovery_add_interest (&discovery, interest_name);
        if (sat_status_get_result (&status) == false)
        {
            fprintf (stderr, "Failed to add interest '%s': %s\n", interest_name, sat_status_get_motive (&status));
            return EXIT_FAILURE;
        }
    }

    status = sat_discovery_start (&discovery);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to start discovery: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }

    // Cleanup
    sleep (10); // Let the discovery run for 10 seconds
    sat_discovery_stop (&discovery);

    return EXIT_SUCCESS;
}