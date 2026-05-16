#include <sat.h>
#include <string.h>
#include <assert.h>

int main (int argc, char *argv[])
{
    sat_file_t sat_file;
    
    char buffer [10] = {0};
    uint32_t size = 0;

    sat_status_t status = sat_file_open (&sat_file, "test", sat_file_mode_write);
    assert (sat_status_get_result (&status) == true);
    status = sat_file_write (&sat_file, "Test", strlen ("Test"));
    assert (sat_status_get_result (&status) == true);
    status = sat_file_close (&sat_file);
    assert (sat_status_get_result (&status) == true);

    status = sat_file_open (&sat_file, "test", sat_file_mode_read);
    assert (sat_status_get_result (&status) == true);
    status = sat_file_read (&sat_file, buffer, sizeof (buffer));
    assert (sat_status_get_result (&status) == true);
    status = sat_file_close (&sat_file);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (buffer, "Test") == 0);

    status = sat_file_open (&sat_file, "test", sat_file_mode_read);
    assert (sat_status_get_result (&status) == true);
    status = sat_file_get_size (&sat_file, &size);
    assert (sat_status_get_result (&status) == true);
    status = sat_file_close (&sat_file);
    assert (sat_status_get_result (&status) == true);

    assert (size == 4);

    status = sat_file_open (&sat_file, "test", sat_file_mode_read);
    assert (sat_status_get_result (&status) == true);
    status = sat_file_readline (&sat_file, buffer, sizeof (buffer));
    assert (sat_status_get_result (&status) == true);
    status = sat_file_readline (&sat_file, buffer, sizeof (buffer));
    assert (sat_status_get_result (&status) == false);
    status = sat_file_close (&sat_file);
    assert (sat_status_get_result (&status) == true);

    return 0;
}