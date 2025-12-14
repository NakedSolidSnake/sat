/**
 * @file sat_iterator.h
 * @brief Generic iterator interface for SAT data structures
 * @author Cristiano Silva de Souza
 * @date 2025
 * 
 * This module provides a unified iterator interface that allows traversing
 * different SAT data structures (arrays, sets, linked lists, etc.) using
 * a common API. The iterator supports both index-based and address-based
 * iteration patterns.
 */

#ifndef SAT_ITERATOR_H_
#define SAT_ITERATOR_H_

#include <sat_status.h>
#include <stdint.h>

/**
 * @brief Iterator traversal type
 * 
 * Defines the method used to traverse the collection.
 */
typedef enum
{
    sat_iterator_type_index,      /**< Index-based iteration (e.g., arrays) */
    sat_iterator_type_address,    /**< Address-based iteration (e.g., linked lists) */
    sat_iterator_type_unknown     /**< Unknown or uninitialized iterator type */
} sat_iterator_type_t;

/**
 * @brief Base structure for iterator-compatible collections
 * 
 * This structure defines the interface that a collection must implement
 * to be compatible with the iterator. Collections can support index-based
 * iteration, address-based iteration, or both.
 */
typedef struct 
{
    void *object;  /**< Pointer to the collection object */
    
    /**
     * @brief Get next element by index (for index-based iteration)
     * @param object Pointer to the collection
     * @param index Current index position
     * @return Pointer to element at index, or NULL if out of bounds
     */
    void *(*next) (const void *const object, const uint32_t index);
    
    /**
     * @brief Get total number of elements (for index-based iteration)
     * @param object Pointer to the collection
     * @return Number of elements in the collection
     */
    uint32_t (*get_amount) (const void *const object);
    
    /**
     * @brief Get starting address (for address-based iteration)
     * @param object Pointer to the collection
     * @return Pointer to first element, or NULL if empty
     */
    void *(*get_address) (const void *const object);
    
    /**
     * @brief Get next address (for address-based iteration)
     * @param object Pointer to the collection
     * @param address Current address/node
     * @return Pointer to next element, or NULL if at end
     */
    void *(*get_next_address) (const void *const object, const void *const address);
    
    /**
     * @brief Extract data from address (for address-based iteration)
     * @param address Pointer to current node/element
     * @return Pointer to the actual data stored in the node
     */
    void *(*get_data) (const void *const address);

} sat_iterator_base_t;

/**
 * @brief Iterator state structure
 * 
 * Maintains the current position and state while iterating through a collection.
 * The iterator automatically determines whether to use index-based or address-based
 * iteration based on the capabilities of the underlying collection.
 */
typedef struct 
{
    uint32_t index;                        /**< Current index position (index-based iteration) */
    const sat_iterator_base_t *base;       /**< Pointer to the collection's base interface */
    uint32_t amount;                       /**< Total number of elements in collection */
    bool initialized;                      /**< Flag indicating if iterator has been initialized */

    sat_iterator_type_t type;              /**< Iteration type being used */
    
    /**
     * @brief Address-based iteration state
     */
    struct
    {
        void *next;                        /**< Next element address */
        void *current;                     /**< Current element address */
    } address;

} sat_iterator_t;

/**
 * @brief Opens and initializes an iterator for a collection
 * 
 * Initializes the iterator state and determines the appropriate iteration
 * strategy (index-based or address-based) based on the collection's capabilities.
 * For index-based iteration, the collection must provide next() and get_amount().
 * For address-based iteration, it must provide get_address(), get_next_address(),
 * and get_data().
 * 
 * @param[out] object Pointer to the iterator structure to initialize
 * @param[in] base Pointer to the collection's base interface
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The base parameter is typically a cast from the collection object
 *       (e.g., (sat_iterator_base_t *)array)
 * @note The iterator does not take ownership of the collection
 * @warning The collection must remain valid for the lifetime of the iterator
 * @see sat_iterator_next()
 */
sat_status_t sat_iterator_open (sat_iterator_t *const object, const sat_iterator_base_t *const base);

/**
 * @brief Retrieves the next element from the collection
 * 
 * Advances the iterator to the next element and returns a pointer to it.
 * The iteration method (index or address-based) is transparent to the caller.
 * Returns NULL when the end of the collection is reached.
 * 
 * @param[in,out] object Pointer to the iterator structure
 * @return Pointer to the next element, or NULL if iteration is complete
 * 
 * @note The returned pointer points directly to data in the collection (zero-copy)
 * @note The pointer remains valid until the collection is modified or destroyed
 * @warning Do not call this function after it returns NULL
 * @warning The iterator must be initialized with sat_iterator_open() before use
 * @see sat_iterator_open()
 */
void *sat_iterator_next (sat_iterator_t *const object);

#endif/* SAT_ITERATOR_H_ */
