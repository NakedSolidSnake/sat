#include <sat_time.h>
#include <string.h>

time_t sat_time_get_epoch_now (void)
{
    time_t now = time (NULL);
    return now;
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