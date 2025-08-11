#ifndef SAT_PROMETHEUS_METRIC_H
#define SAT_PROMETHEUS_METRIC_H

#include <sat_status.h>
#include <time.h>

#define SAT_METRIC_NAME_MAX_LENGTH 64
#define SAT_METRIC_HELP_MAX_LENGTH 256
#define SAT_METRIC_LINE_MAX_LENGTH 512


typedef enum
{
    SAT_METRIC_TYPE_COUNTER,
    SAT_METRIC_TYPE_GAUGE,
    SAT_METRIC_TYPE_SUMMARY,
    SAT_METRIC_TYPE_HISTOGRAM,
    SAT_METRIC_TYPE_INFO

} sat_metric_type_t;

typedef struct 
{
    char name [SAT_METRIC_NAME_MAX_LENGTH + 1];
    char help [SAT_METRIC_HELP_MAX_LENGTH + 1];
    sat_metric_type_t type;
    double value;
    time_t timestamp;

} sat_metric_t;

sat_status_t sat_metric_create (sat_metric_t *metric, const char *name, const char *help, sat_metric_type_t type, double value);

sat_status_t sat_metric_update (sat_metric_t *metric, double value);

sat_status_t sat_metric_to_string (sat_metric_t *metric, char buffer [SAT_METRIC_LINE_MAX_LENGTH + 1]);

#endif /* SAT_PROMETHEUS_METRIC_H */
