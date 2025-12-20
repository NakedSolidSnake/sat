#include <sat_worker.h>
#include <string.h>
#include <stdlib.h>

static sat_status_t sat_worker_is_args_valid (const sat_worker_args_t *const args);
static sat_status_t sat_worker_threads_allocation (sat_worker_t *const object, uint8_t amount);
static sat_status_t sat_worker_threads_start (sat_worker_t *const object);

static void *sat_worker_thread_function (void *const args);

sat_status_t sat_worker_init (sat_worker_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat worker init error: null object");
            break;
        }

        memset (object, 0, sizeof (sat_worker_t));

        if (pthread_mutex_init (&object->mutex, NULL) != 0)
        {
            sat_status_set (&status, false, "sat worker init error: mutex initialization failed");
            break;
        }

        if (pthread_cond_init (&object->cond, NULL) != 0)
        {
            pthread_mutex_destroy (&object->mutex);
            sat_status_set (&status, false, "sat worker init error: condition variable initialization failed");
            break;
        }

    } while (false);

    return status;
}

sat_status_t sat_worker_open (sat_worker_t *const object, const sat_worker_args_t *const args)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            status = sat_status_set (&status, false, "sat worker open error: null object");
            break;
        }

        if (args == NULL)
        {
            status = sat_status_set (&status, false, "sat worker open error: null args");
            break;
        }

        status = sat_worker_is_args_valid (args);
        sat_status_break_on_error (status);

        object->object_size = args->object_size;
        object->threads_amount = args->pool_amount;
        object->handler = args->handler;
        
        status = sat_queue_create (&object->queue, args->object_size);
        sat_status_break_on_error (status);

        status = sat_worker_threads_allocation (object, args->pool_amount);
        sat_status_break_on_error (status);

        status = sat_worker_threads_start (object);
        sat_status_break_on_error (status);
        object->running = true;

        //register on close

    } while (false);

    return status;
}

sat_status_t sat_worker_feed (sat_worker_t *const object, const void *const data)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            status = sat_status_set (&status, false, "sat worker feed error: null object");
            break;
        }

        if (data == NULL)
        {
            status = sat_status_set (&status, false, "sat worker feed error: null data");
            break;
        }

        pthread_mutex_lock (&object->mutex);

        status = sat_queue_enqueue (object->queue, data);

        pthread_cond_signal (&object->cond);
        pthread_mutex_unlock (&object->mutex);

    } while (false);

    return status;
}

sat_status_t sat_worker_close (sat_worker_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat worker close error: null object");
            break;
        }

        object->running = false;
        
        // Wake up all threads
        pthread_mutex_lock (&object->mutex);
        pthread_cond_broadcast (&object->cond);
        pthread_mutex_unlock (&object->mutex);

        if (object->threads != NULL)
        {
            for (uint8_t i = 0; i < object->threads_amount; i++)
            {
                pthread_join (object->threads [i], NULL);
            }

            free (object->threads);
        }

        if (object->queue != NULL)
        {
            sat_queue_destroy (object->queue);
        }

        // Destroy mutex and condition variable
        pthread_mutex_destroy (&object->mutex);
        pthread_cond_destroy (&object->cond);


    } while (false);

    return status;
}

static sat_status_t sat_worker_is_args_valid (const sat_worker_args_t *const args)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (args == NULL)
        {
            sat_status_set (&status, false, "sat worker args is null");
            break;
        }

        if (args->handler == NULL)
        {
            sat_status_set (&status, false, "sat worker args handler is null");
            break;
        }

        if (args->object_size == 0)
        {
            sat_status_set (&status, false, "sat worker args object size is zero");
            break;
        }

        if (args->pool_amount == 0)
        {
            sat_status_set (&status, false, "sat worker args pool amount is zero");
            break;
        }

    } while (false);
 
    return status;
}

static sat_status_t sat_worker_threads_allocation (sat_worker_t *const object, uint8_t amount)
{
    sat_status_t status = sat_status_success (&status);

    object->threads = (pthread_t *) calloc (1, sizeof (pthread_t) * amount);

    if (object->threads == NULL)
    {
        sat_status_failure (&status, "sat worker threads allocation error: memory allocation failed");
    }

    return status;
}

static sat_status_t sat_worker_threads_start (sat_worker_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    for (uint8_t i = 0; i < object->threads_amount; i++)
    {
        if (pthread_create (&object->threads [i], NULL, sat_worker_thread_function, object) != 0)
        {
            sat_status_failure (&status, "sat worker threads start error");
            break;
        }
    }

    return status;
}

static void *sat_worker_thread_function (void *const args)
{
    sat_worker_t *const worker = (sat_worker_t *const)args;
    sat_status_t status;

    uint8_t *const object = (uint8_t *const) malloc (worker->object_size);
    if (object == NULL)
    {
        return NULL;
    }

    while (worker->running)
    {
        pthread_mutex_lock (&worker->mutex);

        memset (object, 0, worker->object_size);
        status = sat_queue_dequeue (worker->queue, object);

        if (sat_status_get_result (&status) == false)
        {
            pthread_cond_wait (&worker->cond, &worker->mutex);
            if (worker->running)
            {
                status = sat_queue_dequeue (worker->queue, object);
            }
        }

        if (sat_status_get_result (&status) == true && worker->running)
        {
            worker->handler (object);
        }

        pthread_mutex_unlock (&worker->mutex);
    }

    free (object);

    return NULL;
}