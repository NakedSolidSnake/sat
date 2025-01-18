#include <sat.h>
#include <string.h>
#include <assert.h>

int main (int argc, char *argv[])
{
    sat_file_t sat_file;
    char buffer [10] = {0};
    uint32_t size = 0;

    assert (sat_file_open (&sat_file, "test", sat_file_mode_write) == true);
    assert (sat_file_write (&sat_file, "Test", strlen ("Test")) == true);
    assert (sat_file_close (&sat_file) == true);

    assert (sat_file_open (&sat_file, "test", sat_file_mode_read) == true);
    assert (sat_file_read (&sat_file, buffer, sizeof (buffer)) == true);
    assert (sat_file_close (&sat_file) == true);

    assert (strcmp (buffer, "Test") == 0);

    assert (sat_file_open (&sat_file, "test", sat_file_mode_read) == true);
    size = sat_file_get_size (&sat_file);
    assert (sat_file_close (&sat_file) == true);

    assert (size == 4);

    assert (sat_file_open (&sat_file, "test", sat_file_mode_read) == true);
    assert (sat_file_readline (&sat_file, buffer, sizeof (buffer)) == true);
    assert (sat_file_readline (&sat_file, buffer, sizeof (buffer)) == false);
    assert (sat_file_close (&sat_file) == true);

    return 0;
}