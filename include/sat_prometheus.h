#ifndef SAT_PROMETHEUS_H
#define SAT_PROMETHEUS_H

#include <sat_status.h>
#include <sat_set.h>
#include <sat_prometheus_metric.h>
#include <time.h>


typedef struct 
{
    char *data;
    uint32_t size;

} sat_prometheus_buffer_t;

typedef struct 
{
    sat_set_t *metrics;  // Set of sat_metric_t

    sat_prometheus_buffer_t buffer;

} sat_prometheus_t;


typedef struct 
{
    char *port;  // Port for the Prometheus server

    sat_prometheus_buffer_t buffer;

} sat_prometheus_args_t;

sat_status_t sat_prometheus_init (sat_prometheus_t *object);

sat_status_t sat_prometheus_open (sat_prometheus_t *object, sat_prometheus_args_t *args);

sat_status_t sat_prometheus_close (sat_prometheus_t *object);

sat_status_t sat_prometheus_register_metric (sat_prometheus_t *object, sat_metric_t *metric);

sat_status_t sat_prometheus_update_metric (sat_prometheus_t *object, sat_metric_t *metric);

sat_status_t sat_prometheus_get_string_metrics (sat_prometheus_t *object, sat_prometheus_buffer_t *buffer);

#endif /* SAT_PROMETHEUS_H */
