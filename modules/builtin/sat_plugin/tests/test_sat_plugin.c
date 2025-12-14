#include <sat.h>
#include <assert.h>
#include <stdio.h>

int main (int argc, char *argv[])
{

    double (*cosine) (double);

    sat_plugin_t plugin;

    sat_status_t status = sat_plugin_open (&plugin, &(sat_plugin_args_t){.library_name = "/lib/x86_64-linux-gnu/libm.so.6"});
    assert (sat_status_get_result (&status) == true);

    status = sat_plugin_load_method (&plugin, "cos", (void *)&cosine);
    assert (sat_status_get_result (&status) == true);

    assert (cosine (0) == 1.0);

    status = sat_plugin_close (&plugin);
    assert (sat_status_get_result (&status) == true);

    return 0;
}