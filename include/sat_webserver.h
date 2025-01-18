#ifndef SAT_WEBSERVER_H_
#define SAT_WEBSERVER_H_

#include <sat_status.h>
#include <sat_array.h>
#include <civetweb.h>

#define SAT_WEBSERVER_HEADERS_AMOUNT        20

typedef enum
{
    sat_webserver_http_status_continue                              = 100,
    sat_webserver_http_status_switching_protocols                   = 101,
    sat_webserver_http_status_early_hints                           = 103,
    sat_webserver_http_status_ok                                    = 200,
    sat_webserver_http_status_created                               = 201,
    sat_webserver_http_status_accepted                              = 202,
    sat_webserver_http_status_non_authoritative_information         = 203,
    sat_webserver_http_status_no_content                            = 204,
    sat_webserver_http_status_reset_content                         = 205,
    sat_webserver_http_status_partial_content                       = 206,
    sat_webserver_http_status_multiple_choices                      = 300,
    sat_webserver_http_status_moved_permanently                     = 301,
    sat_webserver_http_status_found                                 = 302,
    sat_webserver_http_status_see_other                             = 303,
    sat_webserver_http_status_not_modified                          = 304,
    sat_webserver_http_status_temporary_redirect                    = 307,
    sat_webserver_http_status_permanent_redirect                    = 308,
    sat_webserver_http_status_bad_request                           = 400,
    sat_webserver_http_status_unauthorized                          = 401,
    sat_webserver_http_status_payment_required                      = 402,
    sat_webserver_http_status_forbidden                             = 403,
    sat_webserver_http_status_not_found                             = 404,
    sat_webserver_http_status_method_not_allowed                    = 405,
    sat_webserver_http_status_not_acceptable                        = 406,
    sat_webserver_http_status_proxy_authentication_required         = 407,
    sat_webserver_http_status_request_timeout                       = 408,
    sat_webserver_http_status_conflict                              = 409,
    sat_webserver_http_status_gone                                  = 410,
    sat_webserver_http_status_length_required                       = 411,
    sat_webserver_http_status_precondition_failed                   = 412,
    sat_webserver_http_status_payload_too_large                     = 413,
    sat_webserver_http_status_uri_too_long                          = 414,
    sat_webserver_http_status_unsupported_media_type                = 415,
    sat_webserver_http_status_range_not_satisfiable                 = 416,
    sat_webserver_http_status_expectation_failed                    = 417,
    sat_webserver_http_status_im_a_teapot                           = 418,
    sat_webserver_http_status_unprocessable_entity                  = 422,
    sat_webserver_http_status_too_early                             = 425,
    sat_webserver_http_status_upgrade_required                      = 426,
    sat_webserver_http_status_precondition_required                 = 428,
    sat_webserver_http_status_too_many_requests                     = 429,
    sat_webserver_http_status_request_header_fields_too_large       = 431,
    sat_webserver_http_status_unavailable_for_legal_reasons         = 451,
    sat_webserver_http_status_internal_server_error                 = 500,
    sat_webserver_http_status_not_implemented                       = 501,
    sat_webserver_http_status_bad_gateway                           = 502,
    sat_webserver_http_status_service_unavailable                   = 503,
    sat_webserver_http_status_gateway_timeout                       = 504,
    sat_webserver_http_status_http_version_not_supported            = 505,
    sat_webserver_http_status_variant_also_negotiates               = 506,
    sat_webserver_http_status_insufficient_storage                  = 507,
    sat_webserver_http_status_loop_detected                         = 508,
    sat_webserver_http_status_not_extended                          = 510,
    sat_webserver_http_status_network_authentication_required       = 511
} sat_webserver_http_status_t;


typedef int (*sat_webserver_handler_t) (struct mg_connection *connection, void *data);

typedef struct 
{
    struct mg_context *mg_context;
    struct mg_callbacks mg_callbacks;
    char *port;
    char *folder;
    char *threads_amount;
    sat_array_t *array;
} sat_webserver_t;

typedef enum
{
    sat_webserver_mode_static,
    sat_webserver_mode_dynamic,
} sat_webserver_mode_t;

typedef struct 
{
    char *port;
    char *folder;
    char *threads_amount;
    uint16_t endpoint_amount;
    sat_webserver_mode_t mode;
} sat_webserver_args_t;

typedef struct 
{
    struct 
    {
        const char *data;
        uint32_t size;
    } payload;

    struct 
    {
        struct 
        {
            char *key;
            char *value;
        } header [SAT_WEBSERVER_HEADERS_AMOUNT];

        uint8_t amount;

    } headers;

    sat_webserver_http_status_t http_status;

} sat_webserver_response_t;

sat_status_t sat_webserver_init (sat_webserver_t *object);
sat_status_t sat_webserver_open (sat_webserver_t *object, sat_webserver_args_t *args);
sat_status_t sat_webserver_add_endpoint (sat_webserver_t *object, const char *endpoint, const char *method, sat_webserver_handler_t handler, void *data);
sat_status_t sat_webserver_run (sat_webserver_t *object);
sat_status_t sat_webserver_response_set_payload (sat_webserver_response_t *object, char *payload, uint32_t size);
sat_status_t sat_webserver_response_set_status (sat_webserver_response_t *object, sat_webserver_http_status_t http_status);
sat_status_t sat_webserver_response_header_add (sat_webserver_response_t *object, char *key, char *value);
sat_status_t sat_webserver_response_send (struct mg_connection *object, sat_webserver_response_t response);
sat_status_t sat_webserver_close (sat_webserver_t *object);

#endif/* SAT_WEBSERVER_H_ */
