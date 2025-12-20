#include <stdio.h>
#include <sat.h>
#include <assert.h>

int main (int argc, char *argv [])
{
    sat_process_t recover;

    sat_status_t status = sat_process_create (&recover, &(sat_process_args_t)
                                                         {
                                                            .name = "hello_args",
                                                            .args = "Arguments",
                                                            .mode = sat_process_mode_recover
                                                         });
    assert (sat_status_get_result (&status) == true);

    status = sat_process_spawn (&recover);
    assert (sat_status_get_result (&status) == true);

    sat_process_t critical;

    status = sat_process_create (&critical, &(sat_process_args_t)
                                            {
                                            .name = "critical",
                                            .mode = sat_process_mode_critical
                                            });
    assert (sat_status_get_result (&status) == true);

    status = sat_process_spawn (&critical);
    assert (sat_status_get_result (&status) == true);

    sat_process_monitor_t monitor;

    status = sat_process_monitor_open (&monitor);
    assert (sat_status_get_result (&status) == true);
    
    status = sat_process_monitor_add (&monitor, recover);
    assert (sat_status_get_result (&status) == true);

    status = sat_process_monitor_add (&monitor, critical);
    assert (sat_status_get_result (&status) == true);

    status = sat_process_monitor_start (&monitor);
    assert (sat_status_get_result (&status) == true);

    do
    {
        status = sat_process_monitor_scan (&monitor);

    } while (sat_status_get_result (&status) == true);
    

    return 0;
}