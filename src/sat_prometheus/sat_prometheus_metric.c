#include <sat_prometheus_metric.h>
#include <string.h>
#include <stdio.h>
#include <sat_time.h>

static bool sat_metric_is_field_valid (const char *field, size_t max_length);
static bool sat_metric_is_type_valid (sat_metric_type_t type);

sat_status_t sat_metric_create (sat_metric_t *metric, const char *name, const char *help, sat_metric_type_t type, double value)
{
    sat_status_t status = sat_status_set (&status, true, "");

    do
    {
        if (metric == NULL)
        {
            status = sat_status_set (&status, false, "Metric is NULL");
            break;
        }

        if (sat_metric_is_field_valid (name, SAT_METRIC_NAME_MAX_LENGTH) == false)
        {
            status = sat_status_set (&status, false, "Name is invalid");
            break;
        }

        if (sat_metric_is_field_valid (help, SAT_METRIC_HELP_MAX_LENGTH) == false)
        {
            status = sat_status_set (&status, false, "Help is invalid");
            break;
        }

        if (sat_metric_is_type_valid (type) == false)
        {
            status = sat_status_set (&status, false, "Type is invalid");
            break;
        }

        memset (metric, 0, sizeof (sat_metric_t));

        strncpy (metric->name, name, SAT_METRIC_NAME_MAX_LENGTH);
        strncpy (metric->help, help, SAT_METRIC_HELP_MAX_LENGTH);
        metric->type = type;
        metric->value = value;
        metric->timestamp = sat_time_get_utc_epoch_now ();

    } while (false);

    return status;
}

sat_status_t sat_metric_update (sat_metric_t *metric, double value)
{
    sat_status_t status = sat_status_set (&status, true, "");

    do
    {
        if (metric == NULL)
        {
            status = sat_status_set (&status, false, "Metric is NULL");
            break;
        }

        metric->value = value;
        metric->timestamp = sat_time_get_utc_epoch_now ();

    } while (false);

    return status;
}

void sat_metric_to_string (sat_metric_t *metric, char buffer [SAT_METRIC_LINE_MAX_LENGTH + 1])
{
    snprintf (buffer, SAT_METRIC_LINE_MAX_LENGTH,
                "# HELP %s %s\n"
                "# TYPE %s %s\n"
                "%s %.2f\n\n",
                metric->name,
                metric->help,
                metric->name,
                metric->type,
                metric->name,
                metric->value);

}

static bool sat_metric_is_field_valid (const char *field, size_t max_length)
{
    if (field == NULL || strlen (field) == 0 || strlen (field) > max_length)
    {
        return false;
    }

    return true;
}

static bool sat_metric_is_type_valid (sat_metric_type_t type)
{
    if (type < SAT_METRIC_TYPE_COUNTER || type > SAT_METRIC_TYPE_INFO)
    {
        return false;
    }

    return true;
}