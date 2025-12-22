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
    sat_json_t json;

} application_t;

static int send_json_response (struct mg_connection *connection, char *json, sat_webserver_http_status_t http_status);

static int wildcard_handler (struct mg_connection *connection, void *data)
{
    sat_status_t status;
    char *json_string = NULL;
    application_t *app = (application_t *) data;
    sat_webserver_http_status_t http_status = sat_webserver_http_status_internal_server_error;

    const struct mg_request_info *ri = mg_get_request_info (connection);

    do 
    {
        status = sat_json_buffer_clear (&app->json);
        if (sat_status_get_result (&status) == false)
        {
            sat_json_serialize_add (&app->json, sat_json_type_string, "error", (void *)sat_status_get_motive (&status));
            break;
        }

        status = sat_json_serialize_create_object (&app->json);
        if (sat_status_get_result (&status) == false)
        {
            sat_json_serialize_add (&app->json, sat_json_type_string, "error", (void *)sat_status_get_motive (&status));
            break;
        }

        const char *user_agent = mg_get_header (connection, "User-Agent");

        sat_json_serialize_add (&app->json, sat_json_type_string, "message", (void *)"Wildcard endpoint matched!");
        sat_json_serialize_add (&app->json, sat_json_type_string, "method", (void *)ri->request_method ? ri->request_method : "Unknown");
        sat_json_serialize_add (&app->json, sat_json_type_string, "uri", (void *)ri->request_uri ? ri->request_uri : "Unknown");
        sat_json_serialize_add (&app->json, sat_json_type_string, "remote_addr", (void *)ri->remote_addr ? ri->remote_addr : "Unknown");
        sat_json_serialize_add (&app->json, sat_json_type_string, "user_agent", (void *)user_agent ? user_agent : "Unknown");

        http_status = sat_webserver_http_status_ok;
    }
    while (false);
    
    sat_json_to_string (&app->json, &json_string);
    
    return send_json_response (connection, json_string, http_status);
}

static int hello_get_handler (struct mg_connection *connection, void *data)
{
    application_t *app = (application_t *) data;
    sat_status_t status;
    char *json_string = NULL;
    sat_webserver_http_status_t http_status = sat_webserver_http_status_internal_server_error;

    do 
    {
        status = sat_json_buffer_clear (&app->json);
        if (sat_status_get_result (&status) == false)
        {
            sat_json_serialize_add (&app->json, sat_json_type_string, "error", (void *)sat_status_get_motive (&status));
            break;
        }

        sat_json_serialize_create_object (&app->json);
        sat_json_serialize_add (&app->json, sat_json_type_string, "message", (void *)"Hello from specific GET handler!");

        http_status = sat_webserver_http_status_ok;
    } while (false);

    sat_json_to_string (&app->json, &json_string);
    
    return send_json_response (connection, json_string, http_status);
}

static int data_post_handler(struct mg_connection *connection, void *data)
{
    char *json_string = NULL;
    sat_status_t status;
    application_t *app = (application_t *) data;
    sat_webserver_http_status_t http_status = sat_webserver_http_status_internal_server_error;

    do
    {
        status = sat_json_buffer_clear (&app->json);
        if (sat_status_get_result (&status) == false)
        {
            sat_json_serialize_add (&app->json, sat_json_type_string, "error", (void *)sat_status_get_motive (&status));
            break;
        }

        sat_json_serialize_create_object (&app->json);
        sat_json_serialize_add (&app->json, sat_json_type_string, "message", (void *)"Data received via POST!");
        sat_json_serialize_add (&app->json, sat_json_type_string, "status", (void *)"success");

        http_status = sat_webserver_http_status_created;

    } while (false);

    sat_json_to_string (&app->json, &json_string);

    return send_json_response (connection, json_string, http_status);
}

static int shutdown_handler (struct mg_connection *connection, void *data)
{
    char *json_string = NULL;
    sat_status_t status;
    application_t *app = (application_t *) data;
    sat_webserver_http_status_t http_status = sat_webserver_http_status_internal_server_error;
    
    do
    {
        status = sat_json_buffer_clear (&app->json);
        if (sat_status_get_result (&status) == false)
        {
            sat_json_serialize_add (&app->json, sat_json_type_string, "error", (void *)sat_status_get_motive (&status));
            break;
        }

        sat_json_serialize_create_object (&app->json);

        sat_json_serialize_add (&app->json, sat_json_type_string, "message", (void *)"Server shutdown initiated!");
        sat_json_serialize_add (&app->json, sat_json_type_string, "status", (void *)"shutting down");

        http_status = sat_webserver_http_status_ok;

    } while (false);

    sat_json_to_string (&app->json, &json_string);

    sat_webserver_stop (&app->webserver);

    return send_json_response (connection, json_string, http_status);
}

int main (void)
{
    sat_status_t status;
    application_t app = {0};

    printf ("SAT Webserver Wildcard Method Sample\n");
    printf ("====================================\n\n");

    status = sat_json_init (&app.json);
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to initialize JSON: %s\n", sat_status_get_motive (&status));
        return sat_webserver_http_status_internal_server_error;
    }
    
    status = sat_json_open (&app.json, &(sat_json_args_t){.buffer = (char[1024]){0}, .size = sizeof (char[1024])});
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to open JSON: %s\n", sat_status_get_motive (&status));
        return sat_webserver_http_status_internal_server_error;
    }
    
    status = sat_webserver_init (&app.webserver);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to initialize webserver: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }
    
    sat_webserver_args_t args =
    {
        .port = PORT,
        .folder = ".",
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
    
    status = sat_webserver_add_endpoint (&app.webserver, "/hello", "GET", hello_get_handler, &app);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to add /hello GET endpoint: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }
    
    status = sat_webserver_add_endpoint (&app.webserver, "/data", "POST", data_post_handler, &app);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to add /data POST endpoint: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }
    
    status = sat_webserver_add_endpoint (&app.webserver, "/shutdown", "POST", shutdown_handler, &app);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to add /shutdown POST endpoint: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }
    
    status = sat_webserver_add_endpoint (&app.webserver, "/api/wildcard", "*", wildcard_handler, &app);
    if (sat_status_get_result (&status) == false)
    {
        fprintf (stderr, "Failed to add wildcard endpoint: %s\n", sat_status_get_motive (&status));
        return EXIT_FAILURE;
    }
    
    printf ("Listening on port: %s\n", PORT);
    
    printf ("Available endpoints:\n");
    printf ("  GET  /hello          - Specific GET handler\n");
    printf ("  POST /data           - Specific POST handler\n");
    printf ("  POST /shutdown       - Shutdown server handler\n");
    printf ("  *    /api/wildcard   - Wildcard handler (accepts any HTTP method)\n\n");
    
    printf ("Test examples:\n");
    printf ("  curl http://localhost:%s/hello\n", PORT);
    printf ("  curl -X POST http://localhost:%s/data\n", PORT);
    printf ("  curl -X POST http://localhost:%s/shutdown    # Stop server\n", PORT);
    printf ("  curl http://localhost:%s/api/wildcard\n", PORT);
    printf ("  curl -X POST http://localhost:%s/api/wildcard\n", PORT);
    printf ("  curl -X PUT http://localhost:%s/api/wildcard\n", PORT);
    printf ("  curl -X DELETE http://localhost:%s/api/wildcard\n", PORT);
    printf ("  curl -X PATCH http://localhost:%s/api/wildcard\n", PORT);
    printf ("\nTo stop the server, send a POST request to /shutdown\n\n");

    do
    {
        status = sat_webserver_run (&app.webserver);

    } while (sat_status_get_result (&status) == true);
    

    sat_webserver_close (&app.webserver);

    sat_json_close (&app.json);
    
    return EXIT_SUCCESS;
}

static int send_json_response (struct mg_connection *connection, char *json, sat_webserver_http_status_t http_status)
{
    sat_webserver_response_t response = {0};

    sat_webserver_response_set_status (&response, http_status);
    sat_webserver_response_set_payload (&response, json, strlen (json));
    sat_webserver_response_header_add (&response, "Content-Type", "application/json");

    sat_webserver_response_send (connection, response);

    return http_status;
}