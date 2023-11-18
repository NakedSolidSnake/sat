#include <sat_worker.h>
#include <string.h>
#include <stdlib.h>

static sat_status_t sat_worker_is_args_valid (sat_worker_args_t *args);
static sat_status_t sat_worker_threads_allocation (sat_worker_t *object, uint8_t amount);
static sat_status_t sat_worker_threads_start (sat_worker_t *object);

static void *sat_worker_thread_function (void *args);

sat_status_t sat_worker_init (sat_worker_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat worker init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_worker_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_worker_open (sat_worker_t *object, sat_worker_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat worker open error");

    if (object != NULL && args != NULL)
    {
        do 
        {
            status = sat_worker_is_args_valid (args);
            if (sat_status_get_result (&status) == false)
                break;

            object->object_size = args->object_size;
            object->threads_amount = args->pool_amount;
            object->handler = args->handler;
            
            status = sat_queue_create (&object->queue, args->object_size);
            if (sat_status_get_result (&status) == false)
                break;

            status = sat_worker_threads_allocation (object, args->pool_amount);
            if (sat_status_get_result (&status) == false)
                break;

            status = sat_worker_threads_start (object);
            if (sat_status_get_result (&status) == false)
                break;

        } while (false);

        if (sat_status_get_result (&status) == false)
        {
            sat_worker_close (object);
        }

        else
        {
            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_worker_feed (sat_worker_t *object, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat worker feed error");

    if (object != NULL && data != NULL)
    {
        pthread_mutex_lock (&object->mutex);

        status = sat_queue_enqueue (object->queue, data);

        pthread_cond_signal (&object->cond);
        pthread_mutex_unlock (&object->mutex);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_worker_close (sat_worker_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat worker close error");

    if (object != NULL)
    {
        if (object->queue != NULL)
        {
            sat_queue_destroy (object->queue);
        }

        if (object->threads != NULL)
        {
            free (object->threads);
        }

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_worker_is_args_valid (sat_worker_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat worker args error");

    if (args->object_size > 0 && 
        args->pool_amount > 0 && 
        args->handler != NULL)
    {
        sat_status_set (&status, true, "");       
    }

    return status;
}

static sat_status_t sat_worker_threads_allocation (sat_worker_t *object, uint8_t amount)
{
    sat_status_t status = sat_status_set (&status, false, "sat worker threads allocation error");

    object->threads = (pthread_t *) calloc (1, sizeof (pthread_t) * amount);

    if (object->threads != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_worker_threads_start (sat_worker_t *object)
{
    sat_status_t status = sat_status_set (&status, true, "");

    for (uint8_t i = 0; i < object->threads_amount; i++)
    {
        if (pthread_create (&object->threads [i], NULL, sat_worker_thread_function, object) != 0)
        {
            sat_status_set (&status, false, "sat worker threads start error");
            break;
        }
    }

    return status;
}

static void *sat_worker_thread_function (void *args)
{
    sat_worker_t *worker = (sat_worker_t *)args;
    sat_status_t status;

    uint8_t *object = (uint8_t *) malloc (worker->object_size);

    while (true)
    {
        pthread_mutex_lock (&worker->mutex);

        memset (object, 0, worker->object_size);
        status = sat_queue_dequeue (worker->queue, object);

        if (sat_status_get_result (&status) == false)
        {
            pthread_cond_wait (&worker->cond, &worker->mutex);
            status = sat_queue_dequeue (worker->queue, object);
        }

        if (sat_status_get_result (&status) == true)
            worker->handler (object);

        pthread_mutex_unlock (&worker->mutex);
    }

    free (object);

    return NULL;
}