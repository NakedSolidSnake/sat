#ifndef SAT_CURL_H_
#define SAT_CURL_H_

#include <sat_status.h>
#include <curl/curl.h>
#include <stdint.h>

typedef enum 
{
    sat_curl_method_get,
    sat_curl_method_post,
    sat_curl_method_put,
    sat_curl_method_delete
} sat_curl_method_t;


typedef struct 
{
    void *instance;
    struct curl_slist *headers;

    struct 
    {
        char *buffer;
        uint32_t size;
        uint32_t received;
    } response;

} sat_curl_t;

typedef struct 
{
    char *buffer;
    uint32_t size;

} sat_curl_args_t;

sat_status_t sat_curl_init (sat_curl_t *object);
sat_status_t sat_curl_open (sat_curl_t *object, sat_curl_args_t *args);
sat_status_t sat_curl_header_add (sat_curl_t *object, const char *header);
sat_status_t sat_curl_request (sat_curl_t *object, sat_curl_method_t method, const char *url, const char *data);
sat_status_t sat_curl_close (sat_curl_t *object);

#endif/* SAT_CURL_H_ */
