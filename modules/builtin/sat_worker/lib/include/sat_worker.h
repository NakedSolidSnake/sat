/**
 * @file sat_worker.h
 * @brief Thread pool management system for parallel task processing
 * @author SAT Library Team
 * @date December 2025
 * 
 * This module provides a worker thread pool implementation that allows parallel
 * processing of tasks using a queue-based work distribution system. It manages
 * a pool of worker threads that process tasks from a shared queue, providing
 * an efficient way to handle concurrent workloads.
 */

#ifndef SAT_WORKER_H_
#define SAT_WORKER_H_

#include <sat_status.h>
#include <sat_queue.h>
#include <pthread.h>
#include <stdbool.h>

/**
 * @brief Function pointer type for worker task handlers
 * 
 * This callback function is invoked by worker threads to process tasks
 * dequeued from the work queue. The function receives a pointer to the
 * task data and should perform the required processing.
 * 
 * @param object Pointer to the task data to be processed
 */
typedef void (*sat_worker_handler_t) (void *const object);

/**
 * @brief Worker thread pool structure
 * 
 * This structure manages a pool of worker threads that process tasks from
 * a shared queue. It includes synchronization primitives for thread-safe
 * queue access and condition variables for efficient thread coordination.
 */
typedef struct 
{
    pthread_mutex_t mutex;           /**< Mutex for thread synchronization */
    pthread_cond_t cond;             /**< Condition variable for thread signaling */
    pthread_t *threads;              /**< Array of worker thread handles */
    uint8_t threads_amount;          /**< Number of worker threads in the pool */
    sat_queue_t *queue;              /**< Work queue for task distribution */
    uint32_t object_size;            /**< Size of each task object in bytes */
    sat_worker_handler_t handler;    /**< Task processing callback function */
    bool running;                    /**< Flag indicating if worker pool is active */
} sat_worker_t;

/**
 * @brief Configuration parameters for worker thread pool
 * 
 * This structure specifies the configuration for initializing a worker pool,
 * including the number of threads, task size, and processing handler.
 */
typedef struct 
{
    uint8_t pool_amount;             /**< Number of worker threads to create */
    uint32_t object_size;            /**< Size of each task object in bytes */
    sat_worker_handler_t handler;    /**< Callback function for task processing */
} sat_worker_args_t;

/**
 * @brief Initialize a worker thread pool object
 * 
 * Initializes the worker pool structure, setting up synchronization primitives
 * (mutex and condition variable) required for thread-safe operation. This must
 * be called before sat_worker_open().
 * 
 * @param[out] object Pointer to worker pool structure to initialize
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Successfully initialized
 * @retval SAT_STATUS_ERROR Failed to initialize (null object or mutex/cond init failed)
 */
sat_status_t sat_worker_init (sat_worker_t *const object);

/**
 * @brief Open and start the worker thread pool
 * 
 * Creates the work queue, allocates worker threads according to configuration,
 * and starts all threads in the pool. Threads begin waiting for tasks to be
 * fed into the queue.
 * 
 * @param[in,out] object Pointer to initialized worker pool structure
 * @param[in] args Configuration parameters for the worker pool
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Worker pool successfully started
 * @retval SAT_STATUS_ERROR Failed to open (invalid args, allocation failure, or thread creation failed)
 * 
 * @note The handler function must be thread-safe as it will be called concurrently
 * @note pool_amount must be greater than 0
 * @note object_size must be greater than 0
 */
sat_status_t sat_worker_open (sat_worker_t *const object, const sat_worker_args_t *const args);

/**
 * @brief Submit a task to the worker pool for processing
 * 
 * Enqueues a task into the worker pool's queue. One of the worker threads will
 * dequeue and process the task using the registered handler function. This
 * operation is thread-safe and can be called from multiple threads.
 * 
 * @param[in,out] object Pointer to active worker pool
 * @param[in] data Pointer to task data to be processed (will be copied)
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Task successfully enqueued
 * @retval SAT_STATUS_ERROR Failed to enqueue (null object/data or queue full)
 * 
 * @note The data is copied into the queue, so the caller can free/reuse the source
 * @note If all workers are busy, the task waits in the queue
 */
sat_status_t sat_worker_feed (sat_worker_t *const object, const void *const data);

/**
 * @brief Shutdown and cleanup the worker thread pool
 * 
 * Stops all worker threads gracefully by signaling them to exit, waits for
 * all threads to complete their current tasks and terminate, destroys the
 * work queue, and releases all allocated resources.
 * 
 * @param[in,out] object Pointer to worker pool to close
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Worker pool successfully closed
 * @retval SAT_STATUS_ERROR Failed to close (null object)
 * 
 * @note This function blocks until all worker threads have terminated
 * @note Any tasks remaining in the queue will not be processed
 */
sat_status_t sat_worker_close (sat_worker_t *const object);

#endif/* SAT_WORKER_H_ */
