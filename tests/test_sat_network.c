#include <sat_network.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    char ip_address [SAT_NETWORK_IP_MAX_LEN + 1] = {0};

    sat_status_t status = sat_network_get_public_ip (NULL, 0, ip_address);

    assert (sat_status_get_result (&status) == true);
    
    printf ("Public IP Address: %s\n", ip_address);

    return 0;
}