#include <sat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT "8080"
#define THREADS "2"
#define ENDPOINT_AMOUNT 4

static sat_webserver_t webserver;
static volatile int server_should_stop = 0;

static int wildcard_handler (struct mg_connection *connection, void *data)
{
    (void) data;
    
    const struct mg_request_info *ri = mg_get_request_info (connection);
    sat_webserver_response_t response = {0};
    sat_json_t json_obj;
    char json_buffer[1024];
    char *json_string = NULL;
    sat_status_t status;
    
    status = sat_json_init (&json_obj);
    if (sat_status_get_result (&status) == false)
    {
        printf("Failed to initialize JSON: %s\n", sat_status_get_motive(&status));
        return sat_webserver_http_status_internal_server_error;
    }
    
    status = sat_json_open (&json_obj, &(sat_json_args_t){.buffer = json_buffer, .size = sizeof (json_buffer)});
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to open JSON: %s\n", sat_status_get_motive (&status));
        sat_json_close (&json_obj);
        return sat_webserver_http_status_internal_server_error;
    }
    
    status = sat_json_serialize_create_object (&json_obj);
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to create JSON object: %s\n", sat_status_get_motive (&status));
        sat_json_close (&json_obj);
        return sat_webserver_http_status_internal_server_error;
    }
    
    const char *message = "Wildcard endpoint matched!";
    const char *method = ri->request_method ? ri->request_method : "Unknown";
    const char *uri = ri->request_uri ? ri->request_uri : "Unknown";
    const char *remote_addr = ri->remote_addr ? ri->remote_addr : "Unknown";
    const char *user_agent = mg_get_header (connection, "User-Agent");

    if (user_agent == NULL)
    {
        user_agent = "Unknown";
    }
    
    sat_json_serialize_add (&json_obj, sat_json_type_string, "message", (void *)message);
    sat_json_serialize_add (&json_obj, sat_json_type_string, "method", (void *)method);
    sat_json_serialize_add (&json_obj, sat_json_type_string, "uri", (void *)uri);
    sat_json_serialize_add (&json_obj, sat_json_type_string, "remote_addr", (void *)remote_addr);
    sat_json_serialize_add (&json_obj, sat_json_type_string, "user_agent", (void *)user_agent);
    
    status = sat_json_to_string (&json_obj, &json_string);
    if (sat_status_get_result (&status) == false || json_string == NULL)
    {
        printf ("Failed to serialize JSON: %s\n", sat_status_get_motive (&status));
        sat_json_close (&json_obj);
        return sat_webserver_http_status_internal_server_error;
    }
    
    sat_webserver_response_set_status (&response, sat_webserver_http_status_ok);
    sat_webserver_response_set_payload (&response, json_string, strlen (json_string));
    sat_webserver_response_header_add (&response, "Content-Type", "application/json");
    sat_webserver_response_header_add (&response, "Access-Control-Allow-Origin", "*");
    sat_webserver_response_header_add (&response, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, PATCH, OPTIONS");

    sat_webserver_response_send (connection, response);

    printf ("Request handled: %s %s from %s\n", method, uri, remote_addr);

    sat_json_close (&json_obj);

    return sat_webserver_http_status_ok;
}

static int hello_get_handler (struct mg_connection *connection, void *data)
{
    (void) data;
    
    sat_webserver_response_t response = {0};
    sat_json_t json_obj;
    char json_buffer [512];
    char *json_string = NULL;
    sat_status_t status;
    
    status = sat_json_init (&json_obj);
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to initialize JSON: %s\n", sat_status_get_motive (&status));
        return sat_webserver_http_status_internal_server_error;
    }

    status = sat_json_open (&json_obj, &(sat_json_args_t){.buffer = json_buffer, .size = sizeof (json_buffer)});
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to open JSON: %s\n", sat_status_get_motive (&status));
        sat_json_close (&json_obj);
        return sat_webserver_http_status_internal_server_error;
    }
    
    sat_json_serialize_create_object (&json_obj);
    const char *message = "Hello from specific GET handler!";
    sat_json_serialize_add (&json_obj, sat_json_type_string, "message", (void *)message);
    
    status = sat_json_to_string (&json_obj, &json_string);
    if (sat_status_get_result (&status) == false || json_string == NULL)
    {
        printf ("Failed to serialize JSON: %s\n", sat_status_get_motive (&status));
        sat_json_close (&json_obj);
        return sat_webserver_http_status_internal_server_error;
    }
    
    sat_webserver_response_set_status (&response, sat_webserver_http_status_ok);
    sat_webserver_response_set_payload (&response, json_string, strlen (json_string));
    sat_webserver_response_header_add (&response, "Content-Type", "application/json");

    sat_webserver_response_send (connection, response);

    printf ("Hello GET endpoint accessed\n");

    sat_json_close (&json_obj);

    return sat_webserver_http_status_ok;
}

// Handler específico para POST /data
static int data_post_handler(struct mg_connection *connection, void *data)
{
    (void)data;
    
    sat_webserver_response_t response = {0};
    sat_json_t json_obj;
    char json_buffer [512];
    char *json_string = NULL;
    sat_status_t status;
    
    status = sat_json_init (&json_obj);
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to initialize JSON: %s\n", sat_status_get_motive (&status));
        return sat_webserver_http_status_internal_server_error;
    }
    
    status = sat_json_open (&json_obj, &(sat_json_args_t){.buffer = json_buffer, .size = sizeof (json_buffer)});
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to open JSON: %s\n", sat_status_get_motive (&status));
        sat_json_close (&json_obj);
        return sat_webserver_http_status_internal_server_error;
    }
    
    sat_json_serialize_create_object (&json_obj);
    const char *message = "Data received via POST!";
    const char *status_msg = "success";
    sat_json_serialize_add (&json_obj, sat_json_type_string, "message", (void *)message);
    sat_json_serialize_add (&json_obj, sat_json_type_string, "status", (void *)status_msg);
    
    status = sat_json_to_string (&json_obj, &json_string);
    if (sat_status_get_result (&status) == false || json_string == NULL)
    {
        printf ("Failed to serialize JSON: %s\n", sat_status_get_motive (&status));
        sat_json_close (&json_obj);
        return sat_webserver_http_status_internal_server_error;
    }
    
    sat_webserver_response_set_status (&response, sat_webserver_http_status_created);
    sat_webserver_response_set_payload (&response, json_string, strlen (json_string));
    sat_webserver_response_header_add (&response, "Content-Type", "application/json");
    
    sat_webserver_response_send (connection, response);
    
    printf ("Data POST endpoint accessed\n");
    
    sat_json_close (&json_obj);
    
    return sat_webserver_http_status_created;
}

static int custom_generic_handler (struct mg_connection *connection, void *data)
{
    sat_webserver_t *webserver = (sat_webserver_t *)data;
    int status = sat_webserver_http_status_ok;

    uint32_t endpoint_amount;
    sat_array_get_size(webserver->array, &endpoint_amount);

    const struct mg_request_info *ri = mg_get_request_info(connection);

    for (uint32_t i = 0; i < endpoint_amount; i++)
    {
        struct {
            const char *endpoint;
            const char *method;
            int (*handler)(struct mg_connection *, void *);
            void *data;
        } request;

        sat_array_get_object_by(webserver->array, i, &request);

        if (strcmp(request.endpoint, ri->request_uri) == 0 && 
            (strcmp(request.method, ri->request_method) == 0 ||
             strcmp(request.method, "*") == 0))
        {
            status = request.handler(connection, request.data);
            break;
        }
    }

    return status;
}

// Handler para parar o servidor
static int shutdown_handler(struct mg_connection *connection, void *data)
{
    (void)data;
    
    sat_webserver_response_t response = {0};
    sat_json_t json_obj;
    char json_buffer[512];
    char *json_string = NULL;
    sat_status_t status;
    
    // Inicializa e configura o JSON
    status = sat_json_init(&json_obj);
    if (!sat_status_get_result(&status)) {
        printf("Failed to initialize JSON: %s\n", sat_status_get_motive(&status));
        return sat_webserver_http_status_internal_server_error;
    }
    
    status = sat_json_open(&json_obj, &(sat_json_args_t){.buffer = json_buffer, .size = sizeof(json_buffer)});
    if (!sat_status_get_result(&status)) {
        printf("Failed to open JSON: %s\n", sat_status_get_motive(&status));
        sat_json_close(&json_obj);
        return sat_webserver_http_status_internal_server_error;
    }
    
    // Cria objeto JSON e adiciona mensagem
    sat_json_serialize_create_object(&json_obj);
    const char *message = "Server shutdown initiated!";
    const char *status_msg = "shutting down";
    sat_json_serialize_add(&json_obj, sat_json_type_string, "message", (void *)message);
    sat_json_serialize_add(&json_obj, sat_json_type_string, "status", (void *)status_msg);
    
    // Converte para string
    status = sat_json_to_string(&json_obj, &json_string);
    if (!sat_status_get_result(&status) || !json_string) {
        printf("Failed to serialize JSON: %s\n", sat_status_get_motive(&status));
        sat_json_close(&json_obj);
        return sat_webserver_http_status_internal_server_error;
    }
    
    // Configura e envia resposta
    sat_webserver_response_set_status(&response, sat_webserver_http_status_ok);
    sat_webserver_response_set_payload(&response, json_string, strlen(json_string));
    sat_webserver_response_header_add(&response, "Content-Type", "application/json");
    
    sat_webserver_response_send(connection, response);
    
    printf("Shutdown request received - stopping server...\n");
    
    // Sinaliza para parar o servidor
    server_should_stop = 1;
    
    // Limpa recursos
    sat_json_close(&json_obj);
    
    return sat_webserver_http_status_ok;
}

void signal_handler(int sig)
{
    (void)sig;
    printf("\nShutting down webserver...\n");
    sat_webserver_close(&webserver);
    exit(0);
}

int main(void)
{
    sat_status_t status;
    
    printf("SAT Webserver Wildcard Method Sample\n");
    printf("====================================\n\n");
    
    // Inicializa o webserver
    status = sat_webserver_init(&webserver);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "Failed to initialize webserver: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    
    // Abre o webserver com configurações
    sat_webserver_args_t args = {
        .port = PORT,
        .folder = ".",
        .threads_amount = THREADS,
        .endpoint_amount = ENDPOINT_AMOUNT,
        .mode = sat_webserver_mode_dynamic
    };
    
    status = sat_webserver_open(&webserver, &args);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "Failed to open webserver: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    
    // Adiciona endpoints específicos PRIMEIRO (eles têm prioridade)
    status = sat_webserver_add_endpoint(&webserver, "/hello", "GET", hello_get_handler, NULL);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "Failed to add /hello GET endpoint: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    
    status = sat_webserver_add_endpoint(&webserver, "/data", "POST", data_post_handler, NULL);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "Failed to add /data POST endpoint: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    
    // Adiciona endpoint para parar o servidor
    status = sat_webserver_add_endpoint(&webserver, "/shutdown", "POST", shutdown_handler, NULL);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "Failed to add /shutdown POST endpoint: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    
    // Adiciona endpoint wildcard que aceita qualquer método HTTP
    // Este deve ser adicionado POR ÚLTIMO para não sobrescrever os específicos
    status = sat_webserver_add_endpoint(&webserver, "/api/wildcard", "*", wildcard_handler, NULL);
    if (!sat_status_get_result(&status))
    {
        fprintf(stderr, "Failed to add wildcard endpoint: %s\n", sat_status_get_motive(&status));
        return EXIT_FAILURE;
    }
    
    printf("Webserver started successfully!\n");
    printf("Listening on port: %s\n", PORT);
    printf("Threads: %s\n\n", THREADS);
    
    printf("Available endpoints:\n");
    printf("  GET  /hello          - Specific GET handler\n");
    printf("  POST /data           - Specific POST handler\n");
    printf("  POST /shutdown       - Shutdown server handler\n");
    printf("  *    /api/wildcard   - Wildcard handler (accepts any HTTP method)\n\n");
    
    printf("Test examples:\n");
    printf("  curl http://localhost:%s/hello\n", PORT);
    printf("  curl -X POST http://localhost:%s/data\n", PORT);
    printf("  curl -X POST http://localhost:%s/shutdown    # Stop server\n", PORT);
    printf("  curl http://localhost:%s/api/wildcard\n", PORT);
    printf("  curl -X POST http://localhost:%s/api/wildcard\n", PORT);
    printf("  curl -X PUT http://localhost:%s/api/wildcard\n", PORT);
    printf("  curl -X DELETE http://localhost:%s/api/wildcard\n", PORT);
    printf("  curl -X PATCH http://localhost:%s/api/wildcard\n", PORT);
    printf("\nTo stop the server, send a POST request to /shutdown\n\n");
    
    sat_webserver_run (&webserver);

    sat_webserver_close (&webserver);
    
    return EXIT_SUCCESS;
}
