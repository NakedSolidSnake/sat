#include <sat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

static int hello_handler (struct mg_connection *connection, void *data);
static int hello_handler2 (struct mg_connection *connection, void *data);
static int hello_handler_json (struct mg_connection *connection, void *data);
static int parameter_handler (struct mg_connection *connection, void *data);
static int exit_handler (struct mg_connection *connection, void *data);

static void *send_exit_thread (void *args);

int main (int argc, char *argv[])
{
    sat_webserver_t webserver;

    sat_webserver_args_t args = 
    {
        .endpoint_amount = 6,
        .folder = ".",
        .port = "1234",
        .threads_amount = "1"
    };

    pthread_t send_exit;

    pthread_create (&send_exit, NULL, send_exit_thread, NULL);

    sat_status_t status = sat_webserver_init (&webserver);
    assert (sat_status_get_result (&status) == true);

    status = sat_webserver_open (&webserver, &args);
    assert (sat_status_get_result (&status) == true);

    status = sat_webserver_add_endpoint (&webserver, "/", "GET", hello_handler, "Index");
    assert (sat_status_get_result (&status) == true);

    status = sat_webserver_add_endpoint (&webserver, "/hello", "GET", hello_handler, "Hello World");
    assert (sat_status_get_result (&status) == true);

    status = sat_webserver_add_endpoint (&webserver, "/hello2", "GET", hello_handler2, "Hello World 2");
    assert (sat_status_get_result (&status) == true);

    status = sat_webserver_add_endpoint (&webserver, "/hello_json", "GET", hello_handler_json, NULL);
    assert (sat_status_get_result (&status) == true);

    status = sat_webserver_add_endpoint (&webserver, "/greetings", "GET", parameter_handler, NULL);
    assert (sat_status_get_result (&status) == true);

    status = sat_webserver_add_endpoint (&webserver, "/exit", "GET", exit_handler, NULL);
    assert (sat_status_get_result (&status) == true);

    do
    {
        status = sat_webserver_run (&webserver);

    } while (sat_status_get_result (&status) == true);

    status = sat_webserver_close (&webserver);
    assert (sat_status_get_result (&status) == true);

    return 0;
}


static int hello_handler (struct mg_connection *connection, void *data)
{
    sat_webserver_response_send (connection, (sat_webserver_response_t) {.http_status = sat_webserver_http_status_ok});
    return sat_webserver_http_status_ok;
}

static int hello_handler2 (struct mg_connection *connection, void *data)
{
    sat_webserver_response_t response;

    sat_webserver_response_set_payload (&response, (char *)data, strlen (data));
    sat_webserver_response_set_status (&response, sat_webserver_http_status_ok);

    sat_webserver_response_send (connection, response);
    return sat_webserver_http_status_ok;
}

static int hello_handler_json (struct mg_connection *connection, void *data)
{
    (void) data;

    char *json = "{\"message\":\"Hello World\"}";

    sat_webserver_response_t response;

    sat_webserver_response_set_payload (&response, json, strlen (json));
    sat_webserver_response_set_status (&response, sat_webserver_http_status_ok);

    sat_webserver_response_header_add (&response, 
                                       "Content-Type",
                                       "application/json; charset=utf-8");

    sat_webserver_response_send (connection, response);

    return sat_webserver_http_status_ok;
}

static int parameter_handler (struct mg_connection *connection, void *data)
{
    const struct mg_request_info *ri = mg_get_request_info (connection);
    char buffer [100] = {0};
    int status = 404;
    char buffer_send [512] = {0};

    if (ri->query_string != NULL && mg_get_var (ri->query_string, strlen (ri->query_string), "name", buffer, strlen (ri->query_string)) >= 0)
    {
        char *template = "{\"message\":\"Hello %s\"}";

        snprintf (buffer_send, 511, template, buffer);

        sat_webserver_response_t response;

        sat_webserver_response_set_payload (&response, buffer_send, strlen (buffer_send));
        sat_webserver_response_set_status (&response, sat_webserver_http_status_ok);

        sat_webserver_response_header_add (&response, 
                                        "Content-Type",
                                        "application/json; charset=utf-8");

        sat_webserver_response_send (connection, response);

        status = sat_webserver_http_status_ok;
    }

    return status;
}

static int exit_handler (struct mg_connection *connection, void *data)
{
    (void) data;
    (void) connection;

    exit (0);

    return sat_webserver_http_status_ok;
}

static void *send_exit_thread (void *args)
{
    (void) args;

    sleep (1000);

    system ("curl http://localhost:1234/exit");

    return NULL;
}