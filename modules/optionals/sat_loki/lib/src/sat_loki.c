#include <sat_loki.h>
#include <stdarg.h>
#include <sat_curl.h>
#include <unistd.h>
#include <sat_time.h>
#include <string.h>
#include <stdio.h>

#define SAT_LOKI_HOSTNAME_SIZE      64
#define SAT_LOKI_URL_SIZE           256
#define SAT_LOKI_SERVICE_NAME_SIZE  64
#define SAT_LOKI_POR_SIZE           6

typedef struct
{
    sat_curl_t curl;
    char hostname [SAT_LOKI_HOSTNAME_SIZE + 1];
    char url [SAT_LOKI_URL_SIZE + 1];
    char service_name [SAT_LOKI_SERVICE_NAME_SIZE + 1];
    char buffer;
    uint8_t level;
    bool is_open;
} sat_loki_t;

static sat_status_t sat_loki_is_args_valid (const sat_loki_args_t *const args);
static uint8_t sat_loki_get_level_value (const char *const level);

static sat_loki_t loki;

sat_status_t sat_loki_open (const sat_loki_args_t *const args)
{
    sat_status_t status = sat_status_failure (&status, "sat_loki_open: error");

    do
    {
        memset (&loki, 0, sizeof (sat_loki_t));

        status = sat_loki_is_args_valid (args);
        sat_status_break_on_error (status);

        status = sat_curl_init (&loki.curl);
        sat_status_break_on_error (status);

        status = sat_curl_open (&loki.curl, &(sat_curl_args_t)
                                            {
                                                .buffer = &loki.buffer,
                                                .size = sizeof (char)
                                            });
        sat_status_break_on_error (status);

        gethostname (loki.hostname, SAT_LOKI_HOSTNAME_SIZE);
        strncpy (loki.service_name, args->service_name, SAT_LOKI_SERVICE_NAME_SIZE);

        snprintf (loki.url, SAT_LOKI_URL_SIZE, "http://%s:%s/loki/api/v1/push", args->address, args->port);

        loki.level = sat_loki_get_level_value (args->fallback);

        loki.is_open = true;

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_loki_send (const char *const level, const char *fmt, ...)
{
    sat_status_t status = sat_status_failure (&status, "sat_loki_send: error");

    do
    {
        if (loki.is_open == false)
        {
            sat_status_failure (&status, "sat_loki_send: loki is not initialized");
            break;
        }

        char message [1024];
        va_list args;
        va_start (args, fmt);
        vsnprintf (message, sizeof (message), fmt, args);
        va_end (args);

        char payload [2048];

        snprintf (payload, sizeof (payload),
                  "{\"streams\": [{\"stream\": {\"host\": \"%s\", \"service\": \"%s\", \"level\": \"%s\"}, \"values\": [[\"%llu\", \"%s\"]]}]}",
                  loki.hostname,
                  loki.service_name,
                  level != NULL ? level : "error",
                  sat_time_get_epoch_now_ns (),
                  message);

        status = sat_curl_header_add (&loki.curl, "Content-Type: application/json; charset=utf-8");
        sat_status_break_on_error (status);

        status = sat_curl_request (&loki.curl, sat_curl_method_post, loki.url, payload);
        sat_status_break_on_error (status);

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_loki_close (void)
{
    sat_status_t status = sat_status_failure (&status, "sat_loki_close: error");

    do
    {
        if (loki.is_open == false)
        {
            sat_status_failure (&status, "sat_loki_close: loki is not initialized");
            break;
        }

        status = sat_curl_close (&loki.curl);
        sat_status_break_on_error (status);

        loki.is_open = false;

        sat_status_success (&status);

    } while (false);

    return status;  
}

static sat_status_t sat_loki_is_args_valid (const sat_loki_args_t *const args)
{
    sat_status_t status; 

    do
    {
        if (args == NULL)
        {
            sat_status_failure (&status, "sat_loki_is_args_valid: invalid args");
            break;
        }

        if (args->address == NULL)
        {
            sat_status_failure (&status, "sat_loki_is_args_valid: invalid address");
            break;
        }

        if (strlen (args->address) == 0)
        {
            sat_status_failure (&status, "sat_loki_is_args_valid: empty address");
            break;
        }

        if (args->service_name == NULL)
        {
            sat_status_failure (&status, "sat_loki_is_args_valid: invalid service_name");
            break;
        }

        if (strlen (args->service_name) == 0)
        {
            sat_status_failure (&status, "sat_loki_is_args_valid: empty service_name");
            break;
        }

        sat_status_success (&status);

    } while (false);

    return status;
}

static uint8_t sat_loki_get_level_value (const char *const level)
{
    uint8_t value = 1;

    do
    {
        if (level == NULL || strlen (level) == 0 || strcmp (level, "error") == 0)
        {
            break;
        }

        if (strcmp (level, "warning") == 0)
        {
            value = 2;
            break;
        }

        if (strcmp (level, "info") == 0)
        {
            value = 3;
            break;
        }
        
        if (strcmp (level, "debug") == 0)
        {
            value = 4;
            break;
        }

    } while (false);

    return value;
}