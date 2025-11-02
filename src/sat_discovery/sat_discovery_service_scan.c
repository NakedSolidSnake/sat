#include <sat_discovery_services.h>
#include <sat_discovery.h>


void sat_discovery_service_scan (void *object)
{
    sat_discovery_t *discovery = (sat_discovery_t *)object;

    sat_udp_run (&discovery->udp);
}