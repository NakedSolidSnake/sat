/**
 * @file sat_webserver_remove.c
 * @brief Sample demonstrating endpoint addition and removal in SAT webserver
 * 
 * This sample shows how to:
 * - Initialize and configure a webserver
 * - Add multiple endpoints dynamically
 * - Remove specific endpoints by URI and method
 * - Verify endpoint removal by attempting requests
 * 
 * The sample creates a webserver with endpoints for:
 * - /hello - returns a greeting message
 * - /status - returns server status
 * - /remove - removes the /hello endpoint
 * - /shutdown - stops the server
 */

#include <sat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT "8080"
#define THREADS "2"
#define ENDPOINT_AMOUNT 10

/**
 * @brief Application context structure
 */
typedef struct
{
    sat_webserver_t webserver;
    bool hello_endpoint_active;
} application_t;

/**
 * @brief Handler for /hello endpoint
 * 
 * Returns a simple greeting message in JSON format.
 */
static int handler_hello (struct mg_connection *conn, void *data)
{
    sat_webserver_response_t response = {0};
    sat_webserver_response_set_status(&response, sat_webserver_http_status_ok);
    
    char *json_response = "{"
                            "\"message\": \"Hello from SAT Webserver!\","
                            "\"endpoint\": \"/hello\","
                            "\"status\": \"active\""
                            "}";
    
    sat_webserver_response_set_payload(&response, json_response, strlen (json_response));
    sat_webserver_response_header_add(&response, "Content-Type", "application/json");
    
    sat_webserver_response_send(conn, response);
    
    return sat_webserver_http_status_ok;
}

/**
 * @brief Handler for /status endpoint
 * 
 * Returns the current server status including endpoint states.
 */
static int handler_status(struct mg_connection *conn, void *data)
{
    application_t *app = (application_t *)data;
    
    sat_webserver_response_t response = {0};
    sat_webserver_response_set_status(&response, sat_webserver_http_status_ok);
    
    char json_response[512];
    snprintf(json_response, sizeof(json_response),
             "{"
             "\"server\": \"running\","
             "\"hello_endpoint\": \"%s\","
             "\"endpoints\": ["
             "\"/hello\","
             "\"/status\","
             "\"/remove\","
             "\"/shutdown\""
             "]"
             "}",
             app->hello_endpoint_active ? "active" : "removed");
    
    sat_webserver_response_set_payload(&response, json_response, strlen(json_response));
    sat_webserver_response_header_add(&response, "Content-Type", "application/json");
    
    sat_webserver_response_send(conn, response);
    
    return sat_webserver_http_status_ok;
}

/**
 * @brief Handler for /remove endpoint
 * 
 * Removes the /hello endpoint from the server.
 * Note: Endpoint removal requires server restart to take effect.
 */
static int handler_remove(struct mg_connection *conn, void *data)
{
    application_t *app = (application_t *)data;
    
    sat_webserver_response_t response = {0};
    
    sat_status_t status = sat_webserver_remove_endpoint (&app->webserver, "/hello", "GET");
    
    if (sat_status_get_result(&status))
    {
        app->hello_endpoint_active = false;
        
        sat_webserver_response_set_status(&response, sat_webserver_http_status_ok);
        
        char *json_response = "{"
                                "\"status\": \"success\","
                                "\"message\": \"Endpoint /hello removed successfully\","
                                "\"note\": \"Changes will take effect after server restart\""
                                "}";
        
        sat_webserver_response_set_payload(&response, json_response, strlen(json_response));
        
        printf("✓ Endpoint /hello removed successfully\n");
    }
    else
    {
        sat_webserver_response_set_status(&response, sat_webserver_http_status_bad_request);
        
        char json_response[256];
        snprintf(json_response, sizeof(json_response),
                 "{"
                 "\"status\": \"error\","
                 "\"message\": \"%s\""
                 "}",
                 sat_status_get_motive (&status));
        
        sat_webserver_response_set_payload (&response, json_response, strlen(json_response));
        
        fprintf(stderr, "✗ Failed to remove endpoint: %s\n", sat_status_get_motive(&status));
    }
    
    sat_webserver_response_header_add (&response, "Content-Type", "application/json");
    sat_webserver_response_send (conn, response);
    
    return sat_webserver_http_status_ok;
}

/**
 * @brief Handler for /shutdown endpoint
 * 
 * Stops the webserver gracefully.
 */
static int handler_shutdown(struct mg_connection *conn, void *data)
{
    application_t *app = (application_t *)data;
    
    sat_webserver_response_t response = {0};
    sat_webserver_response_set_status(&response, sat_webserver_http_status_accepted);
    
    char *json_response = "{"
                            "\"status\": \"shutting down\","
                            "\"message\": \"Server is shutting down gracefully\""
                            "}";
    
    sat_webserver_response_set_payload (&response, json_response, strlen(json_response));
    sat_webserver_response_header_add (&response, "Content-Type", "application/json");
    
    sat_webserver_response_send(conn, response);
    
    printf("\n✓ Shutdown requested, stopping server...\n");
    
    sat_webserver_stop(&app->webserver);
    
    return sat_webserver_http_status_accepted;
}

/**
 * @brief Main function - Webserver endpoint removal demonstration
 */
int main(int argc, char *argv[])
{
    sat_status_t status;
    application_t app = {0};
    app.hello_endpoint_active = true;
    
    printf("SAT Webserver Endpoint Removal Sample\n");
    printf("======================================\n\n");
    
    // Initialize webserver
    status = sat_webserver_init(&app.webserver);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "✗ Failed to initialize webserver: %s\n", 
                sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    printf("✓ Webserver initialized\n");
    
    // Configure webserver
    sat_webserver_args_t args = {
        .port = PORT,
        .folder = ".",
        .threads_amount = THREADS,
        .endpoint_amount = ENDPOINT_AMOUNT,
        .mode = sat_webserver_mode_dynamic
    };
    
    status = sat_webserver_open(&app.webserver, &args);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "✗ Failed to open webserver: %s\n", 
                sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    printf("✓ Webserver opened on port %s\n", PORT);
    
    // Add endpoints
    printf("\n--- Adding endpoints ---\n");
    
    status = sat_webserver_add_endpoint (&app.webserver, "/hello", "GET", handler_hello, &app);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "✗ Failed to add /hello endpoint: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    printf("✓ Added endpoint: GET /hello\n");
    
    status = sat_webserver_add_endpoint(&app.webserver, "/status", "GET", handler_status, &app);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "✗ Failed to add /status endpoint: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    printf("✓ Added endpoint: GET /status\n");
    
    status = sat_webserver_add_endpoint(&app.webserver, "/remove", "POST", handler_remove, &app);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "✗ Failed to add /remove endpoint: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    printf("✓ Added endpoint: POST /remove\n");
    
    status = sat_webserver_add_endpoint(&app.webserver, "/shutdown", "POST", handler_shutdown, &app);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "✗ Failed to add /shutdown endpoint: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    printf("✓ Added endpoint: POST /shutdown\n");
    
    // Start webserver
    printf("\n--- Starting webserver ---\n");
    status = sat_webserver_run(&app.webserver);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "✗ Failed to start webserver: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    printf("✓ Webserver running on http://localhost:%s\n\n", PORT);
    
    // Display usage instructions
    printf("Available endpoints:\n");
    printf("  GET  http://localhost:%s/hello   - Returns a greeting message\n", PORT);
    printf("  GET  http://localhost:%s/status  - Returns server status\n", PORT);
    printf("  POST http://localhost:%s/remove  - Removes /hello endpoint\n", PORT);
    printf("  POST http://localhost:%s/shutdown - Stops the server\n\n", PORT);
    
    printf("Test commands:\n");
    printf("  curl http://localhost:%s/hello\n", PORT);
    printf("  curl http://localhost:%s/status\n", PORT);
    printf("  curl -X POST http://localhost:%s/remove\n", PORT);
    printf("  curl http://localhost:%s/hello  # Should fail after removal\n", PORT);
    printf("  curl -X POST http://localhost:%s/shutdown\n\n", PORT);
    
    printf("Press Ctrl+C or use /shutdown endpoint to stop the server.\n");
    
    do
    {
        usleep (1);

        status = sat_webserver_run (&app.webserver);
    }
    while (sat_status_get_result (&status) == true);
    
    // Cleanup
    printf("\n--- Cleaning up ---\n");
    status = sat_webserver_close(&app.webserver);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "✗ Failed to close webserver: %s\n", 
                sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    printf("✓ Webserver closed\n");
    
    printf("\n✓ Sample completed successfully\n");
    
    return EXIT_SUCCESS;
}
