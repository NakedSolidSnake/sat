#include <sat_network.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    char ip_address [SAT_NETWORK_IP_MAX_LEN + 1] = {0};

    sat_status_t status = sat_network_get_public_ip (NULL, 0, ip_address);

    assert (sat_status_get_result (&status) == true);

    sat_network_class_t ip_class;

    status = sat_network_get_ip_class (ip_address, &ip_class);
    assert (sat_status_get_result (&status) == true);
    assert (ip_class == sat_network_class_public);

    printf ("Public IP Address: %s\n", ip_address);
    printf ("IP Address Class: %s\n", sat_network_get_ip_class_string (ip_class));

    return 0;
}