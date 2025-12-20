/**
 * @file sat_time.h
 * @brief Time handling and measurement utilities
 * @author SAT Library Team
 * @date December 2025
 * 
 * This module provides comprehensive time handling utilities including epoch time
 * retrieval in various resolutions, time conversions between strings and epochs,
 * and high-precision time measurement for performance profiling and benchmarking.
 */

#ifndef SAT_TIME_H
#define SAT_TIME_H

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Time measurement structure for elapsed time calculations
 * 
 * This structure stores start and stop timestamps using high-resolution
 * clock_gettime() for accurate time measurements. Useful for profiling
 * and performance benchmarking.
 */
typedef struct 
{
    struct timespec start;  /**< Start timestamp */
    struct timespec stop;   /**< Stop timestamp */
} sat_time_t;

/**
 * @brief Get current epoch time in seconds
 * 
 * Returns the current local time as Unix epoch timestamp (seconds since
 * January 1, 1970, 00:00:00 UTC). This is equivalent to calling time(NULL).
 * 
 * @return Current epoch time in seconds
 */
time_t sat_time_get_epoch_now (void);

/**
 * @brief Get current UTC epoch time in seconds
 * 
 * Returns the current time in UTC as Unix epoch timestamp. Unlike
 * sat_time_get_epoch_now(), this function always returns UTC time
 * regardless of the local timezone.
 * 
 * @return Current UTC epoch time in seconds
 */
time_t sat_time_get_utc_epoch_now (void);

/**
 * @brief Get current epoch time in milliseconds
 * 
 * Returns the current time as Unix epoch timestamp with millisecond precision.
 * Useful for applications requiring higher time resolution than seconds.
 * 
 * @return Current epoch time in milliseconds
 */
unsigned long long sat_time_get_epoch_now_ms (void);

/**
 * @brief Get current epoch time in nanoseconds
 * 
 * Returns the current time as Unix epoch timestamp with nanosecond precision.
 * Provides the highest time resolution available for timestamps.
 * 
 * @return Current epoch time in nanoseconds
 */
unsigned long long sat_time_get_epoch_now_ns (void);

/**
 * @brief Convert time string to epoch timestamp
 * 
 * Parses a time string according to the specified format and converts it
 * to Unix epoch timestamp. The format string uses strptime() format specifiers.
 * 
 * @param[in] time_string String representation of time (e.g., "2025-12-20 15:30:00")
 * @param[in] format Format specification string (e.g., "%Y-%m-%d %H:%M:%S")
 * @return Epoch timestamp in seconds, or 0 if parsing fails
 * 
 * @note Common format specifiers:
 *       %Y - 4-digit year, %m - month (01-12), %d - day (01-31)
 *       %H - hour (00-23), %M - minute (00-59), %S - second (00-59)
 */
time_t sat_time_get_epoch_by_string (const char *const time_string, const char *const format);

/**
 * @brief Convert epoch timestamp to formatted date string
 * 
 * Converts a Unix epoch timestamp to a human-readable date/time string
 * according to the specified format. Uses strftime() format specifiers.
 * 
 * @param[out] buffer Buffer to store the formatted date string
 * @param[in] size Size of the buffer in bytes
 * @param[in] format Format specification string (e.g., "%Y-%m-%d %H:%M:%S")
 * @param[in] epoch Unix epoch timestamp to convert
 * @return true on success, false if parameters are invalid
 * 
 * @note Common format specifiers:
 *       %Y - 4-digit year, %m - month (01-12), %d - day (01-31)
 *       %H - hour (00-23), %M - minute (00-59), %S - second (00-59)
 *       %A - full weekday name, %B - full month name
 */
bool sat_time_get_date_by_epoch (char *const buffer, uint8_t size, const char *const format, time_t epoch);

/**
 * @brief Mark the start time for elapsed time measurement
 * 
 * Records the current timestamp as the start point for time measurement.
 * Use sat_time_mark_stop() to record the end point and sat_time_mark_time_elapsed()
 * to calculate the elapsed time. Uses CLOCK_REALTIME for high-precision timing.
 * 
 * @param[out] object Pointer to time measurement structure
 * @return true on success, false if object is NULL or clock_gettime() fails
 * 
 * @see sat_time_mark_stop(), sat_time_mark_time_elapsed()
 */
bool sat_time_mark_start (sat_time_t *const object);

/**
 * @brief Mark the stop time for elapsed time measurement
 * 
 * Records the current timestamp as the stop point for time measurement.
 * Must be called after sat_time_mark_start(). Use sat_time_mark_time_elapsed()
 * to calculate the elapsed time between start and stop.
 * 
 * @param[out] object Pointer to time measurement structure
 * @return true on success, false if object is NULL or clock_gettime() fails
 * 
 * @see sat_time_mark_start(), sat_time_mark_time_elapsed()
 */
bool sat_time_mark_stop (sat_time_t *const object);

/**
 * @brief Calculate elapsed time between start and stop marks
 * 
 * Calculates the time elapsed between sat_time_mark_start() and
 * sat_time_mark_stop() calls with high precision (nanosecond resolution).
 * Both start and stop must be marked before calling this function.
 * 
 * @param[in] object Pointer to time measurement structure with marked start/stop
 * @param[out] time_elapsed Pointer to store elapsed time in seconds (with fractional part)
 * @return true on success, false if parameters are NULL
 * 
 * @note The elapsed time includes both seconds and fractional nanoseconds
 * @see sat_time_mark_start(), sat_time_mark_stop()
 */
bool sat_time_mark_time_elapsed (const sat_time_t *const object, double *const time_elapsed);

#endif/* SAT_TIME_H */
