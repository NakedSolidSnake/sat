#include <sat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT "8080"
#define THREADS "2"
#define ENDPOINT_AMOUNT 4


typedef struct
{
    sat_webserver_t webserver;

} application_t;

int handlers_index (struct mg_connection *conn, void *data)
{
    char path [1024] = {0};

    const struct mg_request_info *ri = mg_get_request_info (conn);

    const char *root = mg_get_option (mg_get_context (conn), "document_root");

    strncpy (path, root, sizeof (path));

    if (strcmp (ri->local_uri, "/") == 0)
    {
        strncat (path, "/index.html", sizeof (path) - strlen (root));
    }
    else 
    {
        strncat (path, ri->local_uri, sizeof (path) - strlen (root));
    }

    mg_send_file (conn, path);

    return sat_webserver_http_status_ok;
}


static int shutdown_handler (struct mg_connection *connection, void *data)
{
    application_t *app = (application_t *) data;

    sat_webserver_stop (&app->webserver);

    return sat_webserver_http_status_accepted;
}

int main (int argc, char *argv[])
{
    sat_status_t status;
    application_t app = {0};

    if (argc != 2)
    {
        fprintf (stderr, "Usage: %s <static folder>\n", argv [0]);
        return EXIT_FAILURE;
    }

    printf ("SAT Webserver Static Sample\n");
    printf ("====================================\n\n");

    status = sat_webserver_init (&app.webserver);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to initialize webserver: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }
    
    sat_webserver_args_t args =
    {
        .port = PORT,
        .folder = argv [1],
        .threads_amount = THREADS,
        .endpoint_amount = ENDPOINT_AMOUNT,
        .mode = sat_webserver_mode_dynamic
    };
    
    status = sat_webserver_open (&app.webserver, &args);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to open webserver: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }
    
    status = sat_webserver_add_endpoint (&app.webserver, "/", "GET", handlers_index, &app);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to add /hello GET endpoint: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }
    
    status = sat_webserver_add_endpoint (&app.webserver, "/shutdown", "POST", shutdown_handler, &app);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to add /shutdown POST endpoint: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }
    
    printf ("Listening on port: %s\n", PORT);
    
    printf ("Available endpoints:\n");
    printf ("  GET  /          - Specific GET handler\n");
    printf ("  POST /shutdown       - Shutdown server handler\n");
    
    printf ("Test examples:\n");
    printf ("\nTo stop the server, send a POST request to /shutdown\n\n");

    do
    {
        status = sat_webserver_run (&app.webserver);

    } while (sat_status_get_result (&status) == true);
    

    sat_webserver_close (&app.webserver);

    return EXIT_SUCCESS;
}
