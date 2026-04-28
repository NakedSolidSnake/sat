#include <sat_worker.h>
#include <string.h>
#include <stdlib.h>

static sat_status_t sat_worker_is_args_valid (const sat_worker_args_t *const args);
static sat_status_t sat_worker_threads_allocation (sat_worker_t *const object, uint8_t amount);
static sat_status_t sat_worker_threads_start (sat_worker_t *const object);

static void *sat_worker_thread_function (void *const args);

sat_status_t sat_worker_init (sat_worker_t *const object)
{
    sat_status_return_on_null (object, "null object");

    memset (object, 0, sizeof (sat_worker_t));

    sat_status_return_on_not_equals (pthread_mutex_init (&object->mutex, NULL), 0, "mutex initialization failed");

    if (pthread_cond_init (&object->cond, NULL) != 0)
    {
        pthread_mutex_destroy (&object->mutex);
        sat_status_return_on_failure ("condition variable initialization failed");
    }

    sat_status_return_on_success ();
}

sat_status_t sat_worker_open (sat_worker_t *const object, const sat_worker_args_t *const args)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (args, "null args");
    sat_status_return_on_error (sat_worker_is_args_valid (args));

    object->object_size = args->object_size;
    object->threads_amount = args->pool_amount;
    object->handler = args->handler;

    sat_status_return_on_error (sat_queue_create (&object->queue, args->object_size));
    sat_status_t status = sat_worker_threads_allocation (object, args->pool_amount);
    if (sat_status_get_result (&status) == false)
    {
        sat_queue_destroy (object->queue);
        sat_status_return_on_error (status);
    }

    status = sat_worker_threads_start (object);
    if (sat_status_get_result (&status) == false)
    {
        free (object->threads);
        sat_queue_destroy (object->queue);
        sat_status_return_on_error (status);
    }

    object->running = true;

    sat_status_return_on_success ();
}

sat_status_t sat_worker_feed (sat_worker_t *const object, const void *const data)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (data, "null data");

    pthread_mutex_lock (&object->mutex);

    sat_status_t status = sat_queue_enqueue (object->queue, data);
    
    pthread_cond_signal (&object->cond);
    pthread_mutex_unlock (&object->mutex);

    return status;
}

sat_status_t sat_worker_close (sat_worker_t *const object)
{
    sat_status_return_on_null (object, "null object");

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

    sat_status_return_on_success ();
}

static sat_status_t sat_worker_is_args_valid (const sat_worker_args_t *const args)
{
    sat_status_return_on_null (args, "args is null");
    sat_status_return_on_null (args->handler, "handler is null");
    sat_status_return_on_equals (args->object_size, 0, "object size is zero");
    sat_status_return_on_equals (args->pool_amount, 0, "pool amount is zero");

    sat_status_return_on_success ();
}

static sat_status_t sat_worker_threads_allocation (sat_worker_t *const object, uint8_t amount)
{
    object->threads = (pthread_t *) calloc (1, sizeof (pthread_t) * amount);

    sat_status_return_on_null (object->threads, "memory allocation failed");

    sat_status_return_on_success ();
}

static sat_status_t sat_worker_threads_start (sat_worker_t *const object)
{
    for (uint8_t i = 0; i < object->threads_amount; i++)
    {
        if (pthread_create (&object->threads [i], NULL, sat_worker_thread_function, object) != 0)
        {
            // If thread creation fails, we need to clean up the threads that were already created
            for (uint8_t j = 0; j < i; j++)
            {
                pthread_join (object->threads [j], NULL);
            }
            
            free (object->threads);
            sat_status_return_on_failure ("thread creation failed");
        }
    }

    sat_status_return_on_success ();
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