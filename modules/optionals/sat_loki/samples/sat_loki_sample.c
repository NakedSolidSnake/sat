#include <sat.h>

int main (void)
{
    sat_loki_args_t loki_args = 
    {
        .address = "localhost",
        .port = "3100",
        .service_name = "sat_loki_sample",
        .fallback = "error"
    };

    sat_status_t status = sat_loki_open (&loki_args);
    if (sat_status_get_result (&status) == false)
    {
        // sat_log_error ("sat_loki_sample: failed to open loki client: %s", sat_status_get_message (&status));
        return -1;
    }

    status = sat_loki_send ("info", "This is a test log message: %d", 1);
    if (sat_status_get_result (&status) == false)
    {
        // sat_log_error ("sat_loki_sample: failed to send log message: %s", sat_status_get_message (&status));
    }

    status = sat_loki_close ();
    if (sat_status_get_result (&status) == false)
    {
        // sat_log_error ("sat_loki_sample: failed to close loki client: %s", sat_status_get_message (&status));
        return -1;
    }

    return 0;
}