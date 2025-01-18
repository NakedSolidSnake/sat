#include <sat.h>
#include <stdio.h>
#include <assert.h>

static void sat_log_logger_custom (sat_log_level_t level, const char *format, va_list list)
{
    vprintf (format, list);
}

int main (int argc, char *argv[])
{
    sat_status_t status;
    sat_log_t log;
    sat_log_t custom;

    status = sat_log_init (&log);
    assert (sat_status_get_result (&status) == true);

    status = sat_log_init (&custom);
    assert (sat_status_get_result (&status) == true);

    status = sat_log_open (&log, &(sat_log_args_t){.type = sat_log_type_default});
    assert (sat_status_get_result (&status) == true);

    status = sat_log_open (&custom, &(sat_log_args_t){.type = sat_log_type_custom, .logger = sat_log_logger_custom});
    assert (sat_status_get_result (&status) == true);

    status = sat_log_registry (&log, sat_log_level_debug, "%s", "Cristiano");
    assert (sat_status_get_result (&status) == true);

    status = sat_log_registry (&custom, sat_log_level_debug, "%s", "Cristiano");
    assert (sat_status_get_result (&status) == true);

    return 0;
}