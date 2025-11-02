#ifndef SAT_DISCOVERY_SERVICES_H
#define SAT_DISCOVERY_SERVICES_H

void sat_discovery_service_scan (void *object);
void sat_discovery_service_announce (void *object);
void sat_discovery_service_heartbeat (void *object);
void sat_discovery_service_interest (void *object);
void sat_discovery_service_node_ageing (void *object);

#endif /* SAT_DISCOVERY_SERVICES_H */
