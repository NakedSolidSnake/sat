#include <sat_prometheus.h>
#include <sat_prometheus_server.h>
#include <stdlib.h>
#include <string.h>
#include <sat_iterator.h>

static bool sat_metric_is_equal (void *element, void *new_element);
static sat_status_t sat_prometheus_is_args_valid (sat_prometheus_args_t *args);
static sat_status_t sat_prometheus_initialize_set (sat_prometheus_t *object, sat_prometheus_args_t *args);

static void sat_prometheus_buffer_start (sat_prometheus_buffer_t *buffer);
static sat_status_t sat_prometheus_build_metrics (sat_prometheus_t *object);

sat_status_t sat_prometheus_init (sat_prometheus_t *object)
{
    sat_status_t status = sat_status_set (&status, true, "");

    do 
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "Object is NULL");
            break;
        }

        memset (object, 0, sizeof (sat_prometheus_t));

    } while (false);
    
    return status;
}

sat_status_t sat_prometheus_open (sat_prometheus_t *object, sat_prometheus_args_t *args)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "Object is NULL");
            break;
        }

        status = sat_prometheus_is_args_valid (args);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_prometheus_initialize_set (object, args);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_prometheus_server_start (object, args);
        if (sat_status_get_result (&status) == false)
        {
            // Clean up if server start fails
            sat_set_destroy (object->metrics);
            object->metrics = NULL;
            break;
        }

        object->buffer.data = args->buffer.data;
        object->buffer.size = args->buffer.size;

    } while (false);

    return status;
}

sat_status_t sat_prometheus_close (sat_prometheus_t *object)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_set(&status, false, "Object is NULL");
            break;
        }

        // Stop the Prometheus server
        status = sat_prometheus_server_stop (object);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        // Destroy the metrics set
        if (object->metrics != NULL)
        {
            status = sat_set_destroy (object->metrics);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }
            object->metrics = NULL;
        }

        memset (object, 0, sizeof (sat_prometheus_t));

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_prometheus_register_metric (sat_prometheus_t *object, sat_metric_t *metric)
{
    sat_status_t status;

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "Object is NULL");
            break;
        }

        if (metric == NULL)
        {
            sat_status_set(&status, false, "Metric is NULL");
            break;
        }

        status = sat_set_add (object->metrics, metric);

    } while (false);
    
    return status;
}

sat_status_t sat_prometheus_update_metric (sat_prometheus_t *object, sat_metric_t *metric)
{
    sat_status_t status = {0};

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "Object is NULL");
            break;
        }

        if (metric == NULL)
        {
            sat_status_set (&status, false, "Metric is NULL");
            break;
        }

        sat_iterator_t iterator;

        status = sat_iterator_open (&iterator, object->metrics);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        sat_status_set (&status, false, "Metric not found");

        sat_metric_t *__metric = sat_iterator_next (&iterator);
        while (__metric != NULL)
        {
            if (strcmp(__metric->name, metric->name) == 0)
            {
                // Update the metric
                sat_metric_update (__metric, metric->value);

                sat_status_set (&status, true, "");

                break;
            }

            __metric = sat_iterator_next (&iterator);
        }

    } while (false);
    
    return status;
}

sat_status_t sat_prometheus_get_string_metrics (sat_prometheus_t *object, sat_prometheus_buffer_t *buffer)
{
    sat_status_t status = sat_status_set (&status, true, "");

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "Object is NULL");
            break;
        }

        if (buffer == NULL)
        {
            sat_status_set (&status, false, "Buffer is NULL or invalid");
            break;
        }

        

        

    } while (false);

    return status;
}

static void sat_prometheus_buffer_start (sat_prometheus_buffer_t *buffer)
{
    memset (buffer->data, 0, buffer->size);

    strncpy (buffer->data, "", buffer->size);
}

static sat_status_t sat_prometheus_build_metrics (sat_prometheus_t *object)
{
    sat_iterator_t iterator;

    do
    {
        sat_status_t status = sat_iterator_open (&iterator, object->metrics);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        sat_prometheus_buffer_clear (&object->buffer);

        sat_metric_t *metric = sat_iterator_next (&iterator);
        while (metric != NULL)
        {
            char metric_string [SAT_METRIC_LINE_MAX_LENGTH + 1];

            sat_metric_to_string (metric, metric_string);

            metric = sat_iterator_next (&iterator);
        }

    } while (false);

    

   
}

// Helper function to compare metrics by name
static bool sat_metric_is_equal (void *element, void *new_element)
{
    sat_metric_t *metric1 = (sat_metric_t *)element;
    sat_metric_t *metric2 = (sat_metric_t *)new_element;
    
    if (metric1 == NULL || metric2 == NULL)
    {
        return false;
    }
    
    return strcmp (metric1->name, metric2->name) == 0;
}

static sat_status_t sat_prometheus_is_args_valid (sat_prometheus_args_t *args)
{
    sat_status_t status = sat_status_set (&status, true, "");

    do
    {
        if (args == NULL)
        {
            status = sat_status_set (&status, false, "Arguments are NULL");
            break;
        }

        if (args->port == NULL || strlen (args->port) == 0)
        {
            status = sat_status_set (&status, false, "Port is NULL or empty");
            break;
        }

        if (args->buffer.data == NULL)
        {
            status = sat_status_set (&status, false, "Buffer is NULL");
            break;
        }

        if (args->buffer.size == 0)
        {
            status = sat_status_set (&status, false, "Buffer size is 0");
            break;
        }

    } while (false);

    return status;
}

static sat_status_t sat_prometheus_initialize_set (sat_prometheus_t *object, sat_prometheus_args_t *args)
{
    sat_status_t status;

    (void) args;

    // Todo extract size and object_size from args if needed
    sat_set_args_t set_args =
    {
        .size = 100,
        .object_size = sizeof (sat_metric_t),
        .is_equal = sat_metric_is_equal,
        .mode = sat_set_mode_dynamic
    };
    
    return sat_set_create (&object->metrics, &set_args);   
}
