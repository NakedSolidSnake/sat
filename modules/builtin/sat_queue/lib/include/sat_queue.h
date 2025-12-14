/**
 * @file sat_queue.h
 * @brief FIFO (First-In-First-Out) queue data structure
 * @author SAT Library Contributors
 * @date 2025
 * 
 * This module provides a dynamic queue implementation supporting standard
 * FIFO operations. The queue can store arbitrary data types and automatically
 * manages memory as elements are enqueued and dequeued.
 */

#ifndef SAT_QUEUE_H_
#define SAT_QUEUE_H_

#include <sat_status.h>
#include <stdint.h>

/**
 * @brief Opaque structure representing a queue
 * 
 * This structure holds the internal state of the queue.
 * Users should not access its fields directly.
 */
typedef struct sat_queue_t sat_queue_t;

/**
 * @brief Callback function type for printing queue elements
 * 
 * Used by sat_queue_debug() to print each element in the queue.
 * 
 * @param element Pointer to the queue element to print
 */
typedef void (*sat_queue_print_t) (const void *const element);

/**
 * @brief Creates a new queue
 * 
 * Allocates and initializes a new queue that can store elements of the
 * specified size.
 * 
 * @param[out] object Pointer to the pointer that will hold the created queue
 * @param[in] object_size Size in bytes of each element to be stored
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The caller is responsible for calling sat_queue_destroy() to free resources
 * @see sat_queue_destroy()
 */
sat_status_t sat_queue_create (sat_queue_t **const object, uint32_t object_size);

/**
 * @brief Adds an element to the back of the queue
 * 
 * Enqueues a new element by copying it to the queue. The element is added
 * to the rear of the queue, following FIFO order.
 * 
 * @param[in,out] object Pointer to the queue
 * @param[in] data Pointer to the data to enqueue
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The data is copied into the queue using memcpy
 * @note Memory is automatically allocated for the new element
 */
sat_status_t sat_queue_enqueue (sat_queue_t *const object, const void *const data);

/**
 * @brief Removes and retrieves the element from the front of the queue
 * 
 * Dequeues the element at the front of the queue and copies it to the
 * provided buffer. The element is removed from the queue following FIFO order.
 * 
 * @param[in,out] object Pointer to the queue
 * @param[out] data Pointer to buffer where the dequeued element will be copied
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning The data buffer must be large enough to hold object_size bytes
 * @note Returns failure if the queue is empty
 * @note The dequeued element is removed from the queue and its memory is freed
 */
sat_status_t sat_queue_dequeue (sat_queue_t *const object, void *const data);

/**
 * @brief Retrieves the current number of elements in the queue
 * 
 * Returns the number of elements currently stored in the queue.
 * 
 * @param[in] object Pointer to the queue
 * @param[out] size Pointer to store the number of elements
 * @return sat_status_t indicating success or failure of the operation
 */
sat_status_t sat_queue_get_size (const sat_queue_t *const object, uint32_t *const size);

/**
 * @brief Prints all elements in the queue for debugging
 * 
 * Traverses the queue from front to back and calls the provided print
 * function for each element. The queue is not modified.
 * 
 * @param[in] object Pointer to the queue
 * @param[in] print Callback function to print each element
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Elements are printed in FIFO order (front to back)
 * @note The print function receives a pointer to each element
 */
sat_status_t sat_queue_debug (const sat_queue_t *const object, sat_queue_print_t print);

/**
 * @brief Destroys the queue and frees all associated resources
 * 
 * Deallocates all memory used by the queue, including all queued elements.
 * 
 * @param[in,out] object Pointer to the queue to destroy
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note After calling this function, the queue pointer becomes invalid
 * @warning Calling any queue functions on a destroyed queue results in undefined behavior
 * @see sat_queue_create()
 */
sat_status_t sat_queue_destroy (sat_queue_t *const object);

#endif/* SAT_QUEUE_H_ */
