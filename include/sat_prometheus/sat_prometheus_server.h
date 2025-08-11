#ifndef SAT_PROMETHEUS_SERVER_H
#define SAT_PROMETHEUS_SERVER_H

#include <sat_status.h>

typedef struct
{
    int dummy;

} sat_prometheus_server_t;

sat_status_t sat_prometheus_server_start (sat_prometheus_server_t *object, const char *port);

sat_status_t sat_prometheus_server_stop (sat_prometheus_server_t *object);

#endif /* SAT_PROMETHEUS_SERVER_H */
