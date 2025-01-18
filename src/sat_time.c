#include <sat_time.h>
#include <string.h>
#include <sys/time.h>

#define SAT_TIME_NANO_TO_SECONDS_FACTOR     1000000000L

static double sat_time_get_seconds (sat_time_t *object);
static double sat_time_get_seconds_from_nanoseconds (sat_time_t *object);

time_t sat_time_get_epoch_now (void)
{
    time_t now = time (NULL);
    return now;
}

unsigned long long sat_time_get_epoch_now_ms (void)
{
    struct timeval tv;

    gettimeofday (&tv, NULL);

    unsigned long long miliseconds = (unsigned long long)(tv.tv_sec) * 1000
                                     +
                                     (unsigned long long)(tv.tv_usec) / 1000;

    return miliseconds;
}

time_t sat_time_get_utc_epoch_now (void)
{
    time_t now = 0;

    time_t _now = time (NULL);
    struct tm *ptm = gmtime (&_now);
    if (ptm != NULL)
    {
        now = mktime (ptm);
    }

    return now;
}

time_t sat_time_get_epoch_by_string (const char *time_string, const char *format)
{
    time_t epoch = 0;

    struct tm tm;

    memset (&tm, 0, sizeof (struct tm));

    if (strptime (time_string, format, &tm) != NULL)
    {
        epoch = mktime (&tm);
    }

    return epoch;
}

bool sat_time_get_date_by_epoch (char *buffer, uint8_t size, const char *format, time_t epoch)
{
    bool status = false;
    struct tm *ptm = NULL;

    if (buffer != NULL && size > 0 && format != NULL)
    {   
        ptm = localtime (&epoch);
        strftime (buffer, size, format, ptm);

        status = true;
    }   

    return status;
}

bool sat_time_mark_start (sat_time_t *object)
{
    bool status = false;

    if (object != NULL)
    {
        status = clock_gettime (CLOCK_REALTIME, &object->start) == 0 ? true : false;
    }

    return status;
}

bool sat_time_mark_stop (sat_time_t *object)
{
    bool status = false;

    if (object != NULL)
    {
        status = clock_gettime (CLOCK_REALTIME, &object->stop) == 0 ? true : false;
    }

    return status;
}

bool sat_time_mark_time_elapsed (sat_time_t *object, double *time_elapsed)
{
     bool status = false;

    if (object != NULL && time_elapsed != NULL)
    {
        *time_elapsed = sat_time_get_seconds (object) + sat_time_get_seconds_from_nanoseconds (object);
        status = true;
    }

    return status;
}

static double sat_time_get_seconds (sat_time_t *object)
{
    return object->stop.tv_sec - object->start.tv_sec;
}

static double sat_time_get_seconds_from_nanoseconds (sat_time_t *object)
{
    return  (object->stop.tv_nsec - object->start.tv_nsec) / (double) SAT_TIME_NANO_TO_SECONDS_FACTOR;
}