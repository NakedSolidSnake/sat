#include <sat.h>
#include <assert.h>
#include <string.h>

int main (int argc, char *argv[])
{
    char *format = "%d/%m/%Y %H:%M:%S";
    char *date = "06/04/2023 21:29:00";

    char buffer [255] = {0};

    time_t time = sat_time_get_epoch_by_string (date, format);

    assert (time == 1680827340);

    sat_time_get_date_by_epoch (buffer, 255, format, time);

    assert (strcmp (date, buffer) == 0);

    return 0;
}