#include <sat.h>
#include <assert.h>
#include <unistd.h>

void no_args (void)
{
    sat_process_t process;

    sat_status_t status = sat_process_create (&process, &(sat_process_args_t)
                                                         {
                                                            .name = "hello",
                                                            .mode = sat_process_mode_no_recover
                                                         });
    assert (sat_status_get_result (&status) == true);

    status = sat_process_spawn (&process);
    assert (sat_status_get_result (&status) == true);

    sleep (1);
}

void with_args (void)
{
    sat_process_t process;

    sat_status_t status = sat_process_create (&process, &(sat_process_args_t)
                                                         {
                                                            .name = "hello_args",
                                                            .args = "Arguments",
                                                            .mode = sat_process_mode_recover
                                                         });
    assert (sat_status_get_result (&status) == true);

    status = sat_process_spawn (&process);
    assert (sat_status_get_result (&status) == true);

    sleep (1);
}

int main (int argc, char *argv [])
{
    no_args ();
    with_args ();

    return 0;
}