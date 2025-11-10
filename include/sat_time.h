#ifndef SAT_TIME_H
#define SAT_TIME_H

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
    struct timespec start;
    struct timespec stop;
} sat_time_t;

time_t sat_time_get_epoch_now (void);
time_t sat_time_get_utc_epoch_now (void);
unsigned long long sat_time_get_epoch_now_ms (void);
unsigned long long sat_time_get_epoch_now_ns (void);

time_t sat_time_get_epoch_by_string (const char *time_string, const char *format);

bool sat_time_get_date_by_epoch (char *buffer, uint8_t size, const char *format, time_t epoch);

bool sat_time_mark_start (sat_time_t *object);
bool sat_time_mark_stop (sat_time_t *object);
bool sat_time_mark_time_elapsed (sat_time_t *object, double *time_elapsed);

#endif/* SAT_TIME_H */
