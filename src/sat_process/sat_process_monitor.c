#include <sat_process_monitor.h>
#include <string.h>
#include <sat_iterator.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

static sat_status_t sat_process_monitor_try_relaunch (sat_process_monitor_t *object, sat_process_t *process);
static sat_status_t sat_process_monitor_relaunch (sat_process_monitor_t *object, sat_process_t *process);
static void sat_process_monitor_killall (sat_process_monitor_t *object);

sat_status_t sat_process_monitor_open (sat_process_monitor_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat process monitor open error");

    if (object != NULL)
    {

        memset (object, 0, sizeof (sat_process_monitor_t));

        status = sat_array_create (&object->array, &(sat_array_args_t)
                                                   {
                                                    .mode = sat_array_mode_dynamic,
                                                    .object_size = sizeof (sat_process_t),
                                                    .size = 1,
                                                   });

        if (sat_status_get_result (&status) == true)
        {
            object->initialized = true;
        }
    }

    return status;
}

sat_status_t sat_process_monitor_add (sat_process_monitor_t *object, sat_process_t process)
{
    sat_status_t status = sat_status_set (&status, false, "sat process monitor add error");

    if (object != NULL && object->initialized == true)
    {
        status = sat_array_add (object->array, (void *)&process);
    }

    return status;
}

sat_status_t sat_process_monitor_start (sat_process_monitor_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat process monitor start error");

    if (object != NULL && object->initialized == true)
    {
        object->start = true;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_process_monitor_scan (sat_process_monitor_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat process monitor scan error");

    if (object != NULL && object->initialized == true && object->start == true)
    {
        sat_iterator_t iterator;

        status = sat_iterator_open (&iterator, (sat_iterator_base_t *)object->array);
        if (sat_status_get_result (&status) == true)
        {
            sat_process_t *process = (sat_process_t *)sat_iterator_next (&iterator);

            while (process != NULL)
            {
                int result;
                pid_t pid = waitpid (process->pid, &result, WNOHANG);

                if (pid > 0)
                {
                    sat_process_monitor_try_relaunch (object, process);
                }

                process = (sat_process_t *)sat_iterator_next (&iterator);
            }
        }

        if (object->start == false)
        {
            sat_status_set (&status, false, "sat process monitor stop service error");
        }
    }

    return status;
}

sat_status_t sat_process_monitor_stop (sat_process_monitor_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat process monitor stop error");

    if (object != NULL && object->initialized == true && object->start == true)
    {
        sat_process_monitor_killall (object);

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_process_monitor_try_relaunch (sat_process_monitor_t *object, sat_process_t *process)
{
    sat_status_t status = sat_status_set (&status, false, "sat process monitor try relaunch error");

    switch (process->mode)
    {
        case sat_process_mode_no_recover:
            // remove
        break;

        case sat_process_mode_recover:
            sat_process_monitor_relaunch (object, process);
        break;

        case sat_process_mode_critical:
            sat_process_monitor_killall (object);
        break;
    }

    return status;
}

static sat_status_t sat_process_monitor_relaunch (sat_process_monitor_t *object, sat_process_t *process)
{
    sat_status_t status = sat_process_spawn (process);

    return status;
}

static void sat_process_monitor_killall (sat_process_monitor_t *object)
{
    sat_iterator_t iterator;

    sat_status_t status = sat_iterator_open (&iterator, (sat_iterator_base_t *)object->array);

    if (sat_status_get_result (&status) == true)
    {
        sat_process_t *process = (sat_process_t *)sat_iterator_next (&iterator);

        while (process != NULL)
        {
            kill (process->pid, SIGKILL);

            process = (sat_process_t *)sat_iterator_next (&iterator);
        }
    }

    object->start = false;
}
