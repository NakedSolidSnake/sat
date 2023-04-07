#ifndef SAT_TIME_H
#define SAT_TIME_H

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

time_t sat_time_get_epoch_now (void);
time_t sat_time_get_utc_epoch_now (void);
time_t sat_time_get_epoch_by_string (const char *time_string, const char *format);

bool sat_time_get_date_by_epoch (char *buffer, uint8_t size, const char *format, time_t epoch);

#endif/* SAT_TIME_H */
