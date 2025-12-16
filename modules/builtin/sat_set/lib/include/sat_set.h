/**
 * @file sat_set.h
 * @brief Set data structure with uniqueness constraint
 * @author SAT Library Contributors
 * @date 2025
 * 
 * This module provides a set implementation that stores unique elements.
 * Duplicate elements are automatically rejected based on a user-defined
 * equality function. The set supports both static (fixed-size) and dynamic
 * (auto-growing) modes.
 */

#ifndef SAT_SET_H_
#define SAT_SET_H_

#include <sat_status.h>
#include <stdint.h>

/**
 * @brief Opaque structure representing a set
 * 
 * This structure holds the internal state of the set.
 * Users should not access its fields directly.
 */
typedef struct sat_set_t sat_set_t;

/**
 * @brief Comparison function type for searching elements
 * 
 * @param element Pointer to the set element being compared
 * @param param Pointer to the search parameter
 * @return true if the element matches the parameter, false otherwise
 */
typedef bool (*sat_set_compare_t) (const void *const element, const void *const param);

/**
 * @brief Equality function type for checking duplicates
 * 
 * Used to determine if two elements are equal, preventing duplicates
 * from being added to the set.
 * 
 * @param element Pointer to the existing element in the set
 * @param new_element Pointer to the element being added
 * @return true if elements are equal (duplicate), false otherwise
 */
typedef bool (*sat_set_is_equal_t) (const void *const element, const void *const new_element);

/**
 * @brief Set growth mode
 * 
 * Defines whether the set has a fixed size or can grow dynamically.
 */
typedef enum
{
    sat_set_mode_static,   /**< Fixed size set - no automatic growth */
    sat_set_mode_dynamic,  /**< Dynamic set - grows automatically when full */
} sat_set_mode_t;

/**
 * @brief Configuration structure for set creation
 * 
 * Contains all parameters needed to create and configure a new set.
 */
typedef struct
{
    uint32_t size;              /**< Initial capacity of the set */
    uint32_t object_size;       /**< Size in bytes of each set element */
    sat_set_is_equal_t is_equal;/**< Function to check element equality */
    sat_set_mode_t mode;        /**< Growth mode (static or dynamic) */
} sat_set_args_t;

/**
 * @brief Creates a new set
 * 
 * Allocates and initializes a new set with the specified configuration.
 * The is_equal function is used to enforce uniqueness.
 * 
 * @param[out] object Pointer to the pointer that will hold the created set
 * @param[in] args Pointer to the configuration structure
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The caller is responsible for calling sat_set_destroy() to free resources
 * @see sat_set_destroy()
 */
sat_status_t sat_set_create (sat_set_t **const object, const sat_set_args_t *args);

/**
 * @brief Adds an element to the set
 * 
 * Attempts to add an element to the set. The element is added only if
 * no equal element already exists (checked via the is_equal function).
 * 
 * @param[in,out] object Pointer to the set object
 * @param[in] data Pointer to the data to add
 * @return sat_status_t indicating success or failure (failure if duplicate)
 * 
 * @note Returns failure if an equal element already exists in the set
 * @note The data is copied into the set using memcpy
 * @note In dynamic mode, the set grows automatically if capacity is reached
 */
sat_status_t sat_set_add (sat_set_t *const object, const void *const data);

/**
 * @brief Updates an element in the set by index
 * 
 * Replaces the element at the given index with new data.
 * 
 * @param[in,out] object Pointer to the set object
 * @param[in] data Pointer to the new data
 * @param[in] index Index of the element to update (0-based)
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning Index must be less than the current set size
 * @note This does not check for duplicates
 */
sat_status_t sat_set_update_by (sat_set_t *const object, const void *const data, uint32_t index);

/**
 * @brief Removes an element from the set by index
 * 
 * Removes the element at the given index and shifts subsequent elements
 * down by one position.
 * 
 * @param[in,out] object Pointer to the set object
 * @param[in] index Index of the element to remove (0-based)
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning Index must be less than the current set size
 * @note This operation has O(n) complexity due to element shifting
 */
sat_status_t sat_set_remove_by (sat_set_t *const object, uint32_t index);

/**
 * @brief Removes an element from the set by matching a parameter
 * 
 * Searches for and removes the first element matching the given parameter
 * using the provided comparison function.
 * 
 * @param[in,out] object Pointer to the set object
 * @param[in] param Pointer to the parameter used for comparison
 * @param[in] compare Function pointer to the comparison function
 * @param[out] data Pointer to store the removed element's data (can be NULL)
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Only the first matching element is removed
 * @see sat_set_compare_t
 */
sat_status_t sat_set_remove_by_parameter (sat_set_t *const object, const void *const param, sat_set_compare_t compare, void *const data);

/**
 * @brief Retrieves an element from the set by index
 * 
 * Copies the element at the given index into the provided data buffer.
 * 
 * @param[in] object Pointer to the set object
 * @param[in] index Index of the element to retrieve (0-based)
 * @param[out] data Pointer to store the retrieved element's data
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning Index must be less than the current set size
 * @warning The data buffer must be large enough to hold object_size bytes
 */
sat_status_t sat_set_get_object_by (const sat_set_t *const object, uint32_t index, void *const data);

/**
 * @brief Retrieves an element from the set by matching a parameter
 * 
 * Searches for the first element matching the given parameter using the
 * provided comparison function, then copies it to the output buffer.
 * 
 * @param[in] object Pointer to the set object
 * @param[in] param Pointer to the parameter used for comparison
 * @param[in] compare Function pointer to the comparison function
 * @param[out] data Pointer to store the retrieved element's data
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Returns the first matching element
 * @see sat_set_compare_t
 */
sat_status_t sat_set_get_object_by_parameter (const sat_set_t *const object, const void *const param, sat_set_compare_t compare, void *const data);


/**
 * @brief Gets a direct reference to an element by matching a parameter
 * 
 * Searches for the first element matching the given parameter using the
 * provided comparison function, then returns a pointer to it without copying.
 * 
 * @param[in] object Pointer to the set object
 * @param[in] param Pointer to the parameter used for comparison
 * @param[in] compare Function pointer to the comparison function
 * @param[out] data Pointer to store the reference to the matched element
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning The returned pointer becomes invalid if the set is resized or destroyed
 * @warning Modifying the element size can corrupt the set structure
 * @note This function provides zero-copy access to set elements
 * @see sat_set_get_object_by_parameter()
 * @see sat_set_compare_t
 */
sat_status_t sat_set_get_object_ref_by_parameter (sat_set_t *const object, const void *const param, sat_set_compare_t compare, void **const data);

/**
 * @brief Retrieves the current size of the set
 * 
 * Returns the number of elements currently stored in the set.
 * 
 * @param[in] object Pointer to the set object
 * @param[out] size Pointer to store the size of the set
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Size represents the number of elements, not the capacity
 */
sat_status_t sat_set_get_size (const sat_set_t *const object, uint32_t *const size);

/**
 * @brief Destroys the set and frees all associated resources
 * 
 * Deallocates all memory used by the set.
 * 
 * @param[in,out] object Pointer to the set object
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note After calling this function, the set pointer becomes invalid
 * @warning Calling any set functions on a destroyed set results in undefined behavior
 * @see sat_set_create()
 */
sat_status_t sat_set_destroy (sat_set_t *const object);

#endif/* SAT_SET_H_ */
