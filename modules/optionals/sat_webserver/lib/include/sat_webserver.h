/**
 * @file sat_webserver.h
 * @brief SAT WebServer Module - HTTP Web Server Implementation
 * 
 * This module provides a complete HTTP web server implementation built on top
 * of CivetWeb library. It supports endpoint registration, request handling,
 * response management, and HTTP status codes.
 * 
 * The webserver supports:
 * - Static and dynamic endpoint registration
 * - HTTP method filtering (GET, POST, etc.)
 * - Custom request handlers
 * - HTTP response with headers and payload
 * - Multi-threaded request processing
 * - File serving capabilities
 * 
 * @author SAT Development Team
 * @date 2025
 * @version 1.0
 */

#ifndef SAT_WEBSERVER_H_
#define SAT_WEBSERVER_H_

#include <sat_array.h>
#include <civetweb.h>

/**
 * @brief Maximum number of HTTP headers per response
 * 
 * This constant defines the maximum number of custom headers that can be
 * added to an HTTP response. If more headers are needed, this value should
 * be increased and the library recompiled.
 */
#define SAT_WEBSERVER_HEADERS_AMOUNT        20

/**
 * @brief HTTP status codes enumeration
 * 
 * This enumeration contains all standard HTTP status codes defined in RFC 7231
 * and other HTTP-related RFCs. These codes are used to indicate the result
 * of HTTP requests and are sent in HTTP responses.
 * 
 * Status codes are grouped into classes:
 * - 1xx: Informational responses
 * - 2xx: Successful responses  
 * - 3xx: Redirection messages
 * - 4xx: Client error responses
 * - 5xx: Server error responses
 */
typedef enum
{
    /* 1xx Informational responses */
    sat_webserver_http_status_continue                              = 100,  /**< Continue processing request */
    sat_webserver_http_status_switching_protocols                   = 101,  /**< Server switching protocols */
    sat_webserver_http_status_early_hints                           = 103,  /**< Early hints response */

    /* 2xx Successful responses */
    sat_webserver_http_status_ok                                    = 200,  /**< Request successful */
    sat_webserver_http_status_created                               = 201,  /**< Resource created */
    sat_webserver_http_status_accepted                              = 202,  /**< Request accepted for processing */
    sat_webserver_http_status_non_authoritative_information         = 203,  /**< Non-authoritative information */
    sat_webserver_http_status_no_content                            = 204,  /**< No content to return */
    sat_webserver_http_status_reset_content                         = 205,  /**< Reset content */
    sat_webserver_http_status_partial_content                       = 206,  /**< Partial content */

    /* 3xx Redirection messages */
    sat_webserver_http_status_multiple_choices                      = 300,  /**< Multiple choices available */
    sat_webserver_http_status_moved_permanently                     = 301,  /**< Resource moved permanently */
    sat_webserver_http_status_found                                 = 302,  /**< Resource found at different URI */
    sat_webserver_http_status_see_other                             = 303,  /**< See other URI */
    sat_webserver_http_status_not_modified                          = 304,  /**< Resource not modified */
    sat_webserver_http_status_temporary_redirect                    = 307,  /**< Temporary redirect */
    sat_webserver_http_status_permanent_redirect                    = 308,  /**< Permanent redirect */

    /* 4xx Client error responses */
    sat_webserver_http_status_bad_request                           = 400,  /**< Bad request syntax */
    sat_webserver_http_status_unauthorized                          = 401,  /**< Authentication required */
    sat_webserver_http_status_payment_required                      = 402,  /**< Payment required */
    sat_webserver_http_status_forbidden                             = 403,  /**< Access forbidden */
    sat_webserver_http_status_not_found                             = 404,  /**< Resource not found */
    sat_webserver_http_status_method_not_allowed                    = 405,  /**< HTTP method not allowed */
    sat_webserver_http_status_not_acceptable                        = 406,  /**< Not acceptable */
    sat_webserver_http_status_proxy_authentication_required         = 407,  /**< Proxy authentication required */
    sat_webserver_http_status_request_timeout                       = 408,  /**< Request timeout */
    sat_webserver_http_status_conflict                              = 409,  /**< Request conflict */
    sat_webserver_http_status_gone                                  = 410,  /**< Resource gone */
    sat_webserver_http_status_length_required                       = 411,  /**< Content length required */
    sat_webserver_http_status_precondition_failed                   = 412,  /**< Precondition failed */
    sat_webserver_http_status_payload_too_large                     = 413,  /**< Payload too large */
    sat_webserver_http_status_uri_too_long                          = 414,  /**< URI too long */
    sat_webserver_http_status_unsupported_media_type                = 415,  /**< Unsupported media type */
    sat_webserver_http_status_range_not_satisfiable                 = 416,  /**< Range not satisfiable */
    sat_webserver_http_status_expectation_failed                    = 417,  /**< Expectation failed */
    sat_webserver_http_status_im_a_teapot                           = 418,  /**< I'm a teapot (RFC 2324) */
    sat_webserver_http_status_unprocessable_entity                  = 422,  /**< Unprocessable entity */
    sat_webserver_http_status_too_early                             = 425,  /**< Too early */
    sat_webserver_http_status_upgrade_required                      = 426,  /**< Upgrade required */
    sat_webserver_http_status_precondition_required                 = 428,  /**< Precondition required */
    sat_webserver_http_status_too_many_requests                     = 429,  /**< Too many requests */
    sat_webserver_http_status_request_header_fields_too_large       = 431,  /**< Request header fields too large */
    sat_webserver_http_status_unavailable_for_legal_reasons         = 451,  /**< Unavailable for legal reasons */

    /* 5xx Server error responses */
    sat_webserver_http_status_internal_server_error                 = 500,  /**< Internal server error */
    sat_webserver_http_status_not_implemented                       = 501,  /**< Not implemented */
    sat_webserver_http_status_bad_gateway                           = 502,  /**< Bad gateway */
    sat_webserver_http_status_service_unavailable                   = 503,  /**< Service unavailable */
    sat_webserver_http_status_gateway_timeout                       = 504,  /**< Gateway timeout */
    sat_webserver_http_status_http_version_not_supported            = 505,  /**< HTTP version not supported */
    sat_webserver_http_status_variant_also_negotiates               = 506,  /**< Variant also negotiates */
    sat_webserver_http_status_insufficient_storage                  = 507,  /**< Insufficient storage */
    sat_webserver_http_status_loop_detected                         = 508,  /**< Loop detected */
    sat_webserver_http_status_not_extended                          = 510,  /**< Not extended */
    sat_webserver_http_status_network_authentication_required       = 511   /**< Network authentication required */
} sat_webserver_http_status_t;


/**
 * @brief HTTP request handler function pointer
 * 
 * This function pointer type defines the signature for HTTP request handlers.
 * Handler functions are called when a registered endpoint receives a matching
 * HTTP request.
 * 
 * @param connection Pointer to the CivetWeb connection structure containing
 *                   request information and used for sending responses
 * @param data       User-defined data pointer that was registered with the endpoint
 * 
 * @return HTTP status code indicating the result of the request processing.
 *         Should return one of the sat_webserver_http_status_t values.
 * 
 * @note The handler function is responsible for:
 *       - Processing the incoming request
 *       - Reading request data if needed
 *       - Sending an appropriate HTTP response
 *       - Returning the correct HTTP status code
 */
typedef int (*sat_webserver_handler_t) (struct mg_connection *connection, void *data);

/**
 * @brief Web server main structure
 * 
 * This structure contains all the necessary data to manage a web server instance.
 * It encapsulates the CivetWeb context, configuration parameters, and endpoint
 * management through an internal array.
 * 
 * @note This structure should be treated as opaque by the user. All access
 *       should be done through the provided API functions.
 */
typedef struct 
{
    struct mg_context *mg_context;      /**< CivetWeb context for server management */
    struct mg_callbacks mg_callbacks;   /**< CivetWeb callback functions */
    char *port;                        /**< Server listening port (e.g., "8080") */
    char *folder;                      /**< Document root folder for static files */
    char *threads_amount;              /**< Number of worker threads (as string) */
    sat_array_t *array;               /**< Array storing registered endpoints */
    bool running;                     /**< Server running state flag */

    struct 
    {
        sat_webserver_handler_t handler; /**< Fallback request handler */
        void *data;                      /**< User data for fallback handler */

    } fallback;

} sat_webserver_t;

/**
 * @brief Web server endpoint management modes
 * 
 * This enumeration defines how the web server manages its endpoint storage:
 * - Static mode: Fixed-size endpoint array, more memory efficient
 * - Dynamic mode: Resizable endpoint array, more flexible
 */
typedef enum
{
    sat_webserver_mode_static,    /**< Static endpoint array allocation */
    sat_webserver_mode_dynamic,   /**< Dynamic endpoint array allocation */
} sat_webserver_mode_t;

/**
 * @brief Web server configuration arguments
 * 
 * This structure contains all the configuration parameters needed to initialize
 * and configure a web server instance. All parameters must be properly set
 * before calling sat_webserver_open().
 */
typedef struct 
{
    char *port;                      /**< Server listening port (e.g., "8080", "443") */
    char *folder;                    /**< Document root folder path for static file serving */
    char *threads_amount;            /**< Number of worker threads (as string, e.g., "10") */
    uint16_t endpoint_amount;        /**< Maximum number of endpoints to support */
    sat_webserver_mode_t mode;       /**< Endpoint storage mode (static/dynamic) */
} sat_webserver_args_t;

/**
 * @brief HTTP response structure
 * 
 * This structure represents an HTTP response that can be sent to a client.
 * It includes the response payload, custom headers, and HTTP status code.
 * 
 * @note The response structure should be initialized and configured using
 *       the provided API functions before sending.
 */
typedef struct 
{
    /**
     * @brief Response payload data
     */
    struct 
    {
        const char *data;    /**< Pointer to response body data */
        uint32_t size;       /**< Size of response body in bytes */
    } payload;

    /**
     * @brief HTTP headers collection
     */
    struct 
    {
        /**
         * @brief Individual HTTP header
         */
        struct 
        {
            char *key;       /**< Header name (e.g., "Content-Type") */
            char *value;     /**< Header value (e.g., "application/json") */
        } header [SAT_WEBSERVER_HEADERS_AMOUNT];

        uint8_t amount;      /**< Number of headers currently set */

    } headers;

    sat_webserver_http_status_t http_status;    /**< HTTP response status code */

} sat_webserver_response_t;

/* ============================================================================
 * Public API Functions
 * ============================================================================ */

/**
 * @brief Initialize a web server instance
 * 
 * Initializes a web server object by clearing its memory and setting up
 * the CivetWeb library. This function must be called before any other
 * web server operations.
 * 
 * @param object Pointer to the web server structure to initialize
 * 
 * @return sat_status_t indicating success or failure:
 *         - Success: Status with result=true
 *         - Failure: Status with result=false and error message
 * 
 * @note This function:
 *       - Zeros out the web server structure
 *       - Initializes the CivetWeb library
 *       - Sets up default callbacks
 * 
 * @see sat_webserver_close()
 */
sat_status_t sat_webserver_init (sat_webserver_t *object);

/**
 * @brief Configure and open a web server instance
 * 
 * Configures the web server with the provided arguments and prepares it
 * for endpoint registration. This function sets up the internal endpoint
 * array and configures server parameters.
 * 
 * @param object Pointer to an initialized web server structure
 * @param args   Pointer to configuration arguments
 * 
 * @return sat_status_t indicating success or failure:
 *         - Success: Status with result=true
 *         - Failure: Status with result=false and error message
 * 
 * @pre object must be initialized with sat_webserver_init()
 * @pre args must contain valid configuration parameters:
 *      - port: Valid port string (e.g., "8080")
 *      - endpoint_amount: > 0
 *      - threads_amount: Valid thread count string
 * 
 * @note This function:
 *       - Copies configuration parameters to the server structure
 *       - Creates the internal endpoint array
 *       - Does not start the server (use sat_webserver_run())
 * 
 * @see sat_webserver_init(), sat_webserver_run(), sat_webserver_close()
 */
sat_status_t sat_webserver_open (sat_webserver_t *object, sat_webserver_args_t *args);

/**
 * @brief Register an HTTP endpoint with the web server
 * 
 * Adds a new HTTP endpoint to the web server's routing table. The endpoint
 * will be matched against incoming requests based on URL path and HTTP method.
 * 
 * @param object   Pointer to an opened web server structure
 * @param endpoint URL path for the endpoint (e.g., "/api/users", "/health")
 * @param method   HTTP method to match (e.g., "GET", "POST", "*" for any)
 * @param handler  Function to handle requests to this endpoint
 * @param data     User data pointer passed to the handler function
 * 
 * @return sat_status_t indicating success or failure:
 *         - Success: Status with result=true
 *         - Failure: Status with result=false and error message
 * 
 * @pre object must be opened with sat_webserver_open()
 * @pre endpoint must be a valid non-empty string
 * @pre method must be a valid non-empty string
 * @pre handler must be a valid function pointer
 * 
 * @note This function:
 *       - Validates the endpoint parameters
 *       - Adds the endpoint to the internal array
 *       - Does not affect a running server until next restart
 * 
 * @see sat_webserver_handler_t, sat_webserver_open()
 */
sat_status_t sat_webserver_add_endpoint (sat_webserver_t *object, const char *endpoint, const char *method, sat_webserver_handler_t handler, void *data);


sat_status_t sat_webserver_remove_endpoint (sat_webserver_t *object, const char *endpoint, const char *method);

sat_status_t sat_webserver_fallback_register (sat_webserver_t *object, sat_webserver_handler_t handler, void *data);

/**
 * @brief Start the web server
 * 
 * Starts the HTTP web server with the configured parameters and registered
 * endpoints. The server will begin listening for incoming connections and
 * processing HTTP requests.
 * 
 * @param object Pointer to an opened web server structure
 * 
 * @return sat_status_t indicating success or failure:
 *         - Success: Status with result=true
 *         - Failure: Status with result=false and error message
 * 
 * @pre object must be opened with sat_webserver_open()
 * @pre At least one endpoint should be registered (recommended)
 * 
 * @note This function:
 *       - Starts the CivetWeb server with configured options
 *       - Registers all endpoints with the server
 *       - Sets the server running state to true
 *       - Returns immediately (server runs in background threads)
 * 
 * @see sat_webserver_open(), sat_webserver_stop(), sat_webserver_add_endpoint()
 */
sat_status_t sat_webserver_run (sat_webserver_t *object);

/**
 * @brief Stop the web server
 * 
 * Stops a running web server. The server will stop accepting new connections
 * and finish processing existing requests before shutting down.
 * 
 * @param object Pointer to a running web server structure
 * 
 * @return sat_status_t indicating success or failure:
 *         - Success: Status with result=true
 *         - Failure: Status with result=false and error message
 * 
 * @pre object must be running (started with sat_webserver_run())
 * 
 * @note This function:
 *       - Sets the server running state to false
 *       - Does not immediately stop the server (graceful shutdown)
 *       - Server threads may continue for a short time
 * 
 * @see sat_webserver_run(), sat_webserver_close()
 */
sat_status_t sat_webserver_stop (sat_webserver_t *object);

/**
 * @brief Set the payload data for an HTTP response
 * 
 * Sets the response body data and size for an HTTP response structure.
 * This data will be sent as the response body when the response is sent.
 * 
 * @param object  Pointer to the response structure
 * @param payload Pointer to the response body data
 * @param size    Size of the response body in bytes
 * 
 * @return sat_status_t indicating success or failure:
 *         - Success: Status with result=true
 *         - Failure: Status with result=false and error message
 * 
 * @pre payload must point to valid data of at least 'size' bytes
 * @pre size must be > 0
 * 
 * @note This function:
 *       - Stores pointers to the payload data (no copy is made)
 *       - The payload data must remain valid until the response is sent
 *       - Content-Length header is automatically set when response is sent
 * 
 * @see sat_webserver_response_send()
 */
sat_status_t sat_webserver_response_set_payload (sat_webserver_response_t *object, char *payload, uint32_t size);

/**
 * @brief Set the HTTP status code for a response
 * 
 * Sets the HTTP status code that will be sent with the response.
 * The status code indicates the result of the HTTP request processing.
 * 
 * @param object      Pointer to the response structure
 * @param http_status HTTP status code to set
 * 
 * @return sat_status_t indicating success or failure:
 *         - Success: Status with result=true
 *         - Failure: Status with result=false and error message
 * 
 * @note Common status codes:
 *       - 200 (OK): Request successful
 *       - 404 (Not Found): Resource not found
 *       - 500 (Internal Server Error): Server error
 * 
 * @see sat_webserver_http_status_t, sat_webserver_response_send()
 */
sat_status_t sat_webserver_response_set_status (sat_webserver_response_t *object, sat_webserver_http_status_t http_status);

/**
 * @brief Add a custom HTTP header to a response
 * 
 * Adds a custom HTTP header to the response. Headers provide additional
 * metadata about the response such as content type, caching directives, etc.
 * 
 * @param object Pointer to the response structure
 * @param key    Header name (e.g., "Content-Type", "Cache-Control")
 * @param value  Header value (e.g., "application/json", "no-cache")
 * 
 * @return sat_status_t indicating success or failure:
 *         - Success: Status with result=true
 *         - Failure: Status with result=false and error message
 * 
 * @pre key and value must be valid non-null strings
 * @pre Response must have room for more headers (< SAT_WEBSERVER_HEADERS_AMOUNT)
 * 
 * @note This function:
 *       - Stores pointers to the header strings (no copy is made)
 *       - Header strings must remain valid until response is sent
 *       - Standard headers like Content-Length are handled automatically
 * 
 * @see SAT_WEBSERVER_HEADERS_AMOUNT, sat_webserver_response_send()
 */
sat_status_t sat_webserver_response_header_add (sat_webserver_response_t *object, char *key, char *value);

/**
 * @brief Send an HTTP response to the client
 * 
 * Sends the configured HTTP response to the client through the provided
 * connection. This includes status code, headers, and payload data.
 * 
 * @param object   Pointer to the CivetWeb connection
 * @param response HTTP response structure containing response data
 * 
 * @return sat_status_t indicating success or failure:
 *         - Success: Status with result=true
 *         - Failure: Status with result=false and error message
 * 
 * @pre object must be a valid CivetWeb connection from a handler function
 * @pre response must be properly configured with status and optional payload/headers
 * 
 * @note This function:
 *       - Sends HTTP status line and headers
 *       - Sends payload data if present
 *       - Sends error response if no payload is set
 *       - Automatically sets Content-Length header
 * 
 * @see sat_webserver_response_set_payload(), sat_webserver_response_set_status()
 */
sat_status_t sat_webserver_response_send (struct mg_connection *object, sat_webserver_response_t response);

/**
 * @brief Close and cleanup a web server instance
 * 
 * Stops the web server (if running) and releases all allocated resources.
 * This function should be called when the web server is no longer needed.
 * 
 * @param object Pointer to the web server structure to close
 * 
 * @return sat_status_t indicating success or failure:
 *         - Success: Status with result=true
 *         - Failure: Status with result=false and error message
 * 
 * @note This function:
 *       - Stops the CivetWeb server
 *       - Destroys the internal endpoint array
 *       - Zeros out the web server structure
 *       - Server object can be reused after calling sat_webserver_init()
 * 
 * @see sat_webserver_init(), sat_webserver_stop()
 */
sat_status_t sat_webserver_close (sat_webserver_t *object);

#endif /* SAT_WEBSERVER_H */
