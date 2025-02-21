#ifndef SAT_PROCESS_MONITOR_H_
#define SAT_PROCESS_MONITOR_H_

#include <sat_array.h>
#include <sat_process.h>

typedef struct 
{
    sat_array_t *array;
    bool initialized;
    bool start;

} sat_process_monitor_t;

sat_status_t sat_process_monitor_open (sat_process_monitor_t *object);
sat_status_t sat_process_monitor_add (sat_process_monitor_t *object, sat_process_t process);
sat_status_t sat_process_monitor_start (sat_process_monitor_t *object);
sat_status_t sat_process_monitor_scan (sat_process_monitor_t *object);
sat_status_t sat_process_monitor_stop (sat_process_monitor_t *object);

#endif/* SAT_PROCESS_MONITOR_H_ */
