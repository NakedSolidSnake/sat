#include <sat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    char *format = "%d/%m/%Y %H:%M:%S";
    char *date = "06/04/2023 21:29:00";
    double time_elapsed;

    char buffer [255] = {0};

    time_t time = sat_time_get_epoch_by_string (date, format);

    assert (time == 1680827340);

    sat_time_get_date_by_epoch (buffer, 255, format, time);

    assert (strcmp (date, buffer) == 0);

    sat_time_t sat_time;
    assert (sat_time_mark_start (&sat_time) == true);

    usleep (5000);

    assert (sat_time_mark_stop (&sat_time) == true);

    assert (sat_time_mark_time_elapsed (&sat_time, &time_elapsed) == true);

    assert (time_elapsed > 0.005);

    return 0;
}