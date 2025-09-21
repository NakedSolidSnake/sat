#include <sat_webserver.h>
#include <string.h>
#include <stdio.h>

typedef struct 
{
    const char *endpoint;
    const char *method;
    sat_webserver_handler_t handler;
    void *data;
} sat_webserver_request_t;

typedef struct 
{
    char *key;
    char *value;
} sat_webserver_header_t;

static int sat_webserver_fallback_handler (struct mg_connection *object, void *data); 

static int sat_webserver_log_message (const struct mg_connection *connection, const char *message);
sat_status_t sat_webserver_is_endpoint_valid (sat_webserver_request_t *object);
static bool sat_webserver_is_payload_present (sat_webserver_response_t *object);
static bool sat_webserver_response_headers_apply (struct mg_connection *object, sat_webserver_response_t *response);

static void sat_webserver_endpoint_register (sat_webserver_t *object);
static int sat_webserver_generic_handler (struct mg_connection *object, void *data);
static int sat_webserver_endpoints_handler (struct mg_connection *object, void *data);
static int sat_webserver_static_handler (struct mg_connection *object, void *data);
static bool sat_webserver_is_extension_supported (const char *extension);

sat_status_t sat_webserver_init (sat_webserver_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_webserver_t));

        mg_init_library (MG_FEATURES_DEFAULT);

        object->mg_callbacks.log_message = sat_webserver_log_message;

        object->fallback.handler = sat_webserver_fallback_handler;
        object->fallback.data = NULL;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_webserver_open (sat_webserver_t *object, sat_webserver_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver open error");

    if (object != NULL && args != NULL && args->port != NULL && args->endpoint_amount > 0 && args->threads_amount != NULL)
    {
        object->port = args->port;
        object->folder = args->folder;
        object->threads_amount = args->threads_amount;

        status = sat_array_create (&object->array, &(sat_array_args_t)
                                                    {
                                                        .size = args->endpoint_amount,
                                                        .object_size = sizeof (sat_webserver_request_t),
                                                        .mode = (sat_array_mode_t)args->mode,
                                                    });

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_webserver_add_endpoint (sat_webserver_t *object, const char *endpoint, const char *method, sat_webserver_handler_t handler, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver add enpoint error");

    if (object != NULL)
    {
        sat_webserver_request_t request = 
        {
            .endpoint = endpoint,
            .method = method,
            .handler = handler,
            .data = data
        };

        status = sat_webserver_is_endpoint_valid (&request);

        if (sat_status_get_result (&status) == true)
        {
            status = sat_array_add (object->array, &request);
        }        
    }

    return status;
}

sat_status_t sat_webserver_fallback_register (sat_webserver_t *object, sat_webserver_handler_t handler, void *data)
{
    sat_status_t status;

    do 
    {
        if (object == NULL)
        {
            status = sat_status_set (&status, false, "sat webserver fallback register error: sat_webserver_t is NULL");
            break;
        }

        if (handler == NULL)
        {
            status = sat_status_set (&status, false, "sat webserver fallback register error: handler is NULL");
            break;
        }

        object->fallback.handler = handler;
        object->fallback.data = data;

    } while (false);

    return status;
}

sat_status_t sat_webserver_run (sat_webserver_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver run error");

    if (object != NULL && object->running == false)
    {
        const char *options [] = 
        {
            "listening_ports",
            object->port,
            "request_timeout_ms",
            "10000",
            "error_log_file",
            "error.log",
            "enable_auth_domain_check",
            "no",
            "document_root",            
            object->folder,
            "num_threads",
            object->threads_amount,
            0
        };

        object->mg_context = mg_start (&object->mg_callbacks, 0, options);

        if (object->mg_context != NULL)
        {
            sat_webserver_endpoint_register (object);

            object->running = true;

            sat_status_set (&status, true, "");
        }
    }
    else if (object != NULL && object->running == true)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_webserver_stop (sat_webserver_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver stop error");

    if (object != NULL && object->running == true)
    {
        object->running = false;
        
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_webserver_response_set_payload (sat_webserver_response_t *object, char *payload, uint32_t size)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver init error");

    if (object != NULL && payload != NULL && size > 0)
    {
        object->payload.data = payload;
        object->payload.size = size;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_webserver_response_set_status (sat_webserver_response_t *object, sat_webserver_http_status_t http_status)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver init error");

    if (object != NULL)
    {
        object->http_status = http_status;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_webserver_response_header_add (sat_webserver_response_t *object, char *key, char *value)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver init error");

    if (object != NULL && object->headers.amount < SAT_WEBSERVER_HEADERS_AMOUNT && key != NULL && value != NULL)
    {
        uint8_t i = object->headers.amount;

        object->headers.header [i].key = key;
        object->headers.header [i].value = value;

        object->headers.amount ++;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_webserver_response_send (struct mg_connection *object, sat_webserver_response_t response)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver init error");

    if (object != NULL)
    {

        if (sat_webserver_is_payload_present (&response) == true)
        {
            char size_string [10] = {0};

            mg_response_header_start (object, response.http_status);

            sat_webserver_response_headers_apply (object, &response);

            snprintf (size_string, 9, "%d", response.payload.size);

            mg_response_header_add (object, "Content-Length", size_string, -1);

            mg_response_header_send (object);

            mg_write (object, response.payload.data, response.payload.size);
        }

        else 
        {
            mg_send_http_error (object, response.http_status, "%s", "");
        }

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_webserver_close (sat_webserver_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver init error");

    if (object != NULL)
    {
        mg_stop (object->mg_context);

        sat_array_destroy (object->array);

        memset (object, 0, sizeof (sat_webserver_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

static int sat_webserver_log_message (const struct mg_connection *connection, const char *message)
{
    return 1;
}

sat_status_t sat_webserver_is_endpoint_valid (sat_webserver_request_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat webserver enpoint error");

    if (object->endpoint != NULL && 
        strlen (object->endpoint) > 0 &&
        object->method != NULL &&
        strlen (object->method) > 0 &&
        object->handler != NULL)
    {
        sat_status_set (&status, true, "");
    } 

    return status;
}

static bool sat_webserver_is_payload_present (sat_webserver_response_t *object)
{
    bool status = false;

    if (object->payload.data != NULL &&
        object->payload.size > 0)
    {
        status = true;
    }

    return status;
}

static bool sat_webserver_response_headers_apply (struct mg_connection *object, sat_webserver_response_t *response)
{
    bool status = false;

    if (response->headers.amount > 0)
    {
        for (uint8_t i = 0; i < response->headers.amount; i++)
        {
            mg_response_header_add (object,
                                    response->headers.header [i].key,
                                    response->headers.header [i].value,
                                    -1);
        }

        status = true;
    }

    return status;
}

static void sat_webserver_endpoint_register (sat_webserver_t *object)
{
    mg_set_request_handler (object->mg_context,
                            "/",
                            sat_webserver_generic_handler,
                            object);
}

static int sat_webserver_generic_handler (struct mg_connection *object, void *data)
{
    sat_webserver_t *webserver = (sat_webserver_t *)data;
    int status;

    do
    {
        status = sat_webserver_static_handler (object, data);
        if (status != sat_webserver_http_status_not_found)
        {
            break;
        }

        status = sat_webserver_endpoints_handler (object, data);
        if (status == sat_webserver_http_status_not_found)
        {
            webserver->fallback.handler (object, webserver->fallback.data);
        }

    } while (false);

    return status;
}

static int sat_webserver_endpoints_handler (struct mg_connection *object, void *data)
{
    sat_webserver_t *webserver = (sat_webserver_t *)data;
    int status = sat_webserver_http_status_not_found;

    uint32_t endpoint_amount;
    sat_array_get_size (webserver->array, &endpoint_amount);

    const struct mg_request_info *ri = mg_get_request_info (object);

    for (uint32_t i = 0; i < endpoint_amount; i++)
    {
        sat_webserver_request_t request;

        sat_array_get_object_by (webserver->array, i, &request);

        if (strcmp (request.endpoint, ri->request_uri) == 0 && 
            (strcmp (request.method, ri->request_method) == 0 ||
             strcmp (request.method, "*") == 0))
        {
            status = request.handler (object, request.data);
            break;
        }
    }

    return status;
}

static int sat_webserver_static_handler (struct mg_connection *object, void *data)
{
    int http_status = sat_webserver_http_status_not_found;

    (void) data;

    const struct mg_request_info *ri = mg_get_request_info (object);

    const char *root = mg_get_option (mg_get_context (object), "document_root");

    if (sat_webserver_is_extension_supported (ri->request_uri) == true) 
    {
        char path[1024] = {0};

        snprintf (path, sizeof (path), "%s%s", root, ri->request_uri);

        mg_send_file (object, path);

        http_status = sat_webserver_http_status_ok;
    }

    return http_status;
}

static int sat_webserver_fallback_handler (struct mg_connection *object, void *data)
{
    (void) data;

    sat_webserver_response_t response;
    
    memset (&response, 0, sizeof (sat_webserver_response_t));

    sat_webserver_response_set_payload (&response, "Not Found", 9);
    sat_webserver_response_set_status (&response, sat_webserver_http_status_not_found);

    sat_webserver_response_send (object, response);

    return sat_webserver_http_status_not_found;
}

static bool sat_webserver_is_extension_supported (const char *uri)
{
    bool status = false;

    const char *extension = strrchr (uri, '.');

    const char *supported_extensions [] = 
    {
        ".html",
        ".css",
        ".js",
        ".png",
        ".jpg",
        ".jpeg",
        ".ico"
    };

    for (uint8_t i = 0; i < sizeof (supported_extensions) / sizeof (supported_extensions [0]) && extension != NULL; i++)
    {
        if (strcmp (extension, supported_extensions [i]) == 0)
        {
            status = true;
            break;
        }
    }

    return status;
}