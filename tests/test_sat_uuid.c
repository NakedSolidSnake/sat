#include <sat.h>
#include <string.h>
#include <assert.h>

int main (int argc, char *argv[])
{
    char uuid [37] = {0};

    sat_uuid_generate (uuid, sat_uuid_format_upper_case);

    assert (strlen (uuid) == 36);
    
    return 0;
}
