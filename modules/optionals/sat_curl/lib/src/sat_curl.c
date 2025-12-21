#include <sat_curl.h>
#include <string.h>

static void sat_curl_set_method (sat_curl_t *object, sat_curl_method_t method, const char *data);
static size_t sat_curl_read_callback (char *data, size_t size, size_t nmemb, void *user);

sat_status_t sat_curl_init (sat_curl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat curl init error");
    CURLcode code;

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_curl_t));

        code = curl_global_init (CURL_GLOBAL_DEFAULT);

        code  == CURLE_OK ? sat_status_set (&status, true, "") : status;
    }

    return status;
}

sat_status_t sat_curl_open (sat_curl_t *object, sat_curl_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat curl open error");

    if (object != NULL && args != NULL && args->buffer != NULL && args->size > 0)
    {
        object->instance = curl_easy_init ();

        if (object->instance != NULL)
        {
            object->response.buffer = args->buffer;
            object->response.size = args->size;

            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_curl_header_add (sat_curl_t *object, const char *header)
{
    sat_status_t status = sat_status_set (&status, false, "sat curl header add error");

    if (object != NULL && header != NULL)
    {
        object->headers = curl_slist_append (object->headers, header);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_curl_request (sat_curl_t *object, sat_curl_method_t method, const char *url, const char *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat curl request error");

    if (object != NULL && url != NULL)
    {
        CURLcode code;

        curl_easy_reset (object->instance);
        curl_easy_setopt (object->instance, CURLOPT_HTTPHEADER, object->headers);
        curl_easy_setopt (object->instance, CURLOPT_URL, url);
        curl_easy_setopt (object->instance, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt (object->instance, CURLOPT_FORBID_REUSE, 1);

        sat_curl_set_method (object, method, data);

        memset (object->response.buffer, 0, object->response.size);
        object->response.received = 0;

        code = curl_easy_perform (object->instance);

        code == CURLE_OK ? sat_status_set (&status, true, "") : status;

        curl_slist_free_all (object->headers);
        curl_easy_cleanup (object->instance);
    }

    return status;
}

sat_status_t sat_curl_close (sat_curl_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat curl close error");

    if (object != NULL)    
    {
        curl_global_cleanup ();
        sat_status_set (&status, true, "");
    }

    return status;
}

static void sat_curl_set_method (sat_curl_t *object, sat_curl_method_t method, const char *data)
{
    switch (method)
    {
    case sat_curl_method_get:
        curl_easy_setopt (object->instance, CURLOPT_WRITEFUNCTION, sat_curl_read_callback);
        curl_easy_setopt (object->instance, CURLOPT_WRITEDATA, object);
        break;

    case sat_curl_method_post:
        curl_easy_setopt (object->instance, CURLOPT_POST, 1L);
        curl_easy_setopt (object->instance, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt (object->instance, CURLOPT_POSTFIELDSIZE_LARGE, strlen (data));
        curl_easy_setopt (object->instance, CURLOPT_READFUNCTION, sat_curl_read_callback);
        curl_easy_setopt (object->instance, CURLOPT_READDATA, object);

        break;

    case sat_curl_method_put:
        curl_easy_setopt (object->instance, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt (object->instance, CURLOPT_CUSTOMREQUEST, "PUT");
        break;

    case sat_curl_method_delete:
        curl_easy_setopt (object->instance, CURLOPT_CUSTOMREQUEST, "DELETE");
        break;
    
    default:
        break;
    }
}

static size_t sat_curl_read_callback (char *data, size_t size, size_t nmemb, void *user)
{
    sat_curl_t *object = (sat_curl_t *)user;

    size_t bytes = size * nmemb;

    if ((object->response.received + bytes) < object->response.size)
    {
        memcpy (&object->response.buffer [object->response.received], data, bytes);

        object->response.received += bytes;
        object->response.buffer [object->response.received] = 0;
    }

    else 
        bytes = 0;

    return bytes;
}