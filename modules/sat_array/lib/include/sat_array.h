/**
 * @file sat_array.h
 * @brief Dynamic array data structure with static and dynamic growth modes
 * @author Cristiano Silva de Souza
 * @date 2025
 * 
 * This module provides a flexible dynamic array implementation that supports
 * both static (fixed-size) and dynamic (auto-growing) modes. The array can
 * store arbitrary data types and provides common operations like add, remove,
 * update, and search.
 */

#ifndef SAT_ARRAY_H_
#define SAT_ARRAY_H_

#include <sat_status.h>
#include <stdint.h>

/**
 * @brief Opaque structure representing a dynamic array
 * 
 * This structure holds the internal state of the dynamic array.
 * Users should not access its fields directly.
 */
typedef struct sat_array_t sat_array_t;

/**
 * @brief Comparison function type for array search operations
 * 
 * @param element Pointer to the array element being compared
 * @param param Pointer to the search parameter
 * @return true if the element matches the parameter, false otherwise
 */
typedef bool (*sat_array_compare_t) (const void *const element, const void *const param);

/**
 * @brief Callback function type for memory allocation notifications
 * 
 * This callback is invoked when the array grows in dynamic mode.
 * 
 * @param user User-defined context pointer
 * @param new_size New capacity of the array after growth
 */
typedef void (*sat_array_memory_notify_t) (void *const user, uint32_t new_size);

/**
 * @brief Array growth mode
 * 
 * Defines whether the array has a fixed size or can grow dynamically.
 */
typedef enum
{
    sat_array_mode_static,   /**< Fixed size array - no automatic growth */
    sat_array_mode_dynamic,  /**< Dynamic array - grows automatically when full */
} sat_array_mode_t;

/**
 * @brief Configuration structure for array creation
 * 
 * Contains all parameters needed to create and configure a new array.
 */
typedef struct
{
    uint32_t size;              /**< Initial capacity of the array */
    uint32_t object_size;       /**< Size in bytes of each array element */
    sat_array_mode_t mode;      /**< Growth mode (static or dynamic) */
    
    /**
     * @brief Memory growth notification configuration
     */
    struct
    {
        sat_array_memory_notify_t on_increase;  /**< Callback invoked on array growth */
        void *user;                              /**< User context passed to callback */
    } notification;

} sat_array_args_t;

/**
 * @brief Creates a new sat_array_t object
 * 
 * Allocates and initializes a new dynamic array with the specified configuration.
 * 
 * @param[out] object Pointer to the pointer that will hold the created sat_array_t object
 * @param[in] args Pointer to the sat_array_args_t structure containing the array configuration
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The caller is responsible for calling sat_array_destroy() to free resources
 * @see sat_array_destroy()
 */
sat_status_t sat_array_create (sat_array_t **const object, const sat_array_args_t *const args);

/**
 * @brief Adds an object to the array
 * 
 * Appends a new element to the end of the array. In dynamic mode, the array
 * will automatically grow if full. In static mode, addition fails if capacity
 * is reached.
 * 
 * @param[in,out] object Pointer to the sat_array_t object
 * @param[in] data Pointer to the data to be added
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The data is copied into the array using memcpy
 */
sat_status_t sat_array_add (sat_array_t *const object, const void *const data);

/**
 * @brief Updates an object in the array at the specified index
 * 
 * Replaces the element at the given index with new data.
 * 
 * @param[in,out] object Pointer to the sat_array_t object
 * @param[in] data Pointer to the new data to update
 * @param[in] index Index of the object to update (0-based)
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning Index must be less than the current array size
 */
sat_status_t sat_array_update_by (sat_array_t *const object, const void *const data, const uint32_t index);

/**
 * @brief Removes an object from the array at the specified index
 * 
 * Removes the element at the given index and shifts all subsequent elements
 * down by one position.
 * 
 * @param[in,out] object Pointer to the sat_array_t object
 * @param[in] index Index of the object to remove (0-based)
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning Index must be less than the current array size
 * @note This operation has O(n) complexity due to element shifting
 */
sat_status_t sat_array_remove_by (sat_array_t *const object, const uint32_t index);

/**
 * @brief Removes an object from the array by matching a parameter using a comparison function
 * 
 * Searches for the first element that matches the given parameter using the
 * provided comparison function, then removes it.
 * 
 * @param[in,out] object Pointer to the sat_array_t object
 * @param[in] param Pointer to the parameter used for comparison
 * @param[in] compare Function pointer to the comparison function
 * @param[out] data Pointer to store the removed object's data. If NULL, the data will be discarded
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Only the first matching element is removed
 * @see sat_array_compare_t
 */
sat_status_t sat_array_remove_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare ,void *const data);

/**
 * @brief Retrieves an object from the array at the specified index
 * 
 * Copies the element at the given index into the provided data buffer.
 * 
 * @param[in] object Pointer to the sat_array_t object
 * @param[in] index Index of the object to retrieve (0-based)
 * @param[out] data Pointer to store the retrieved object's data
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning Index must be less than the current array size
 * @warning The data buffer must be large enough to hold object_size bytes
 */
sat_status_t sat_array_get_object_by (const sat_array_t *const object, const uint32_t index, void *const data);

/**
 * @brief Retrieves an object from the array by matching a parameter using a comparison function
 * 
 * Searches for the first element that matches the given parameter using the
 * provided comparison function, then copies it to the output buffer.
 * 
 * @param[in] object Pointer to the sat_array_t object
 * @param[in] param Pointer to the parameter used for comparison
 * @param[in] compare Function pointer to the comparison function
 * @param[out] data Pointer to store the retrieved object's data
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Returns the first matching element
 * @see sat_array_compare_t
 */
sat_status_t sat_array_get_object_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare, void *const data);

/**
 * @brief Gets a direct reference to an object by matching a parameter using a comparison function
 * 
 * Searches for the first element that matches the given parameter using the
 * provided comparison function, then returns a pointer to it without copying.
 * This allows for efficient read/write access to matching array elements.
 * 
 * @param[in] object Pointer to the sat_array_t object
 * @param[in] param Pointer to the parameter used for comparison
 * @param[in] compare Function pointer to the comparison function
 * @param[out] data Pointer to store the reference to the matched object
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning The returned pointer becomes invalid if the array is resized or destroyed
 * @warning Modifying the element size can corrupt the array structure
 * @note This function provides zero-copy access to array elements
 * @note Returns a reference to the first matching element
 * @see sat_array_compare_t
 * @see sat_array_get_object_by_parameter()
 */
sat_status_t sat_array_get_object_ref_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare, void **const data);

/**
 * @brief Retrieves the current size of the array
 * 
 * Returns the number of elements currently stored in the array.
 * 
 * @param[in] object Pointer to the sat_array_t object
 * @param[out] size Pointer to store the size of the array
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Size represents the number of elements, not the capacity
 * @see sat_array_get_capacity()
 */
sat_status_t sat_array_get_size (const sat_array_t *const object, uint32_t *const size);

/**
 * @brief Clears all objects from the array
 * 
 * Removes all elements from the array, resetting the size to 0.
 * The capacity remains unchanged.
 * 
 * @param[in,out] object Pointer to the sat_array_t object
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note This does not free the allocated memory, only resets the element count
 */
sat_status_t sat_array_clear (sat_array_t *const object);

/**
 * @brief Retrieves the capacity of the array
 * 
 * Returns the maximum number of elements the array can hold without reallocation.
 * 
 * @param[in] object Pointer to the sat_array_t object
 * @param[out] capacity Pointer to store the capacity of the array
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Capacity may be larger than size in dynamic mode
 * @see sat_array_get_size()
 */
sat_status_t sat_array_get_capacity (const sat_array_t *const object, uint32_t *const capacity);

/**
 * @brief Destroys the array and frees associated resources
 * 
 * Deallocates all memory used by the array and sets the pointer to NULL.
 * 
 * @param[in,out] object Pointer to the sat_array_t object
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note After calling this function, the object pointer becomes invalid
 * @warning Calling any array functions on a destroyed array results in undefined behavior
 */
sat_status_t sat_array_destroy (sat_array_t *const object);

/**
 * @brief Gets a direct reference to an array element at the specified index
 * 
 * Returns a pointer to the element stored at the given index without copying.
 * This allows for efficient read/write access to array elements.
 * 
 * @param[in] object Pointer to the sat_array_t object
 * @param[in] index Index of the element to reference (0-based)
 * @return Pointer to the element at the specified index, or NULL if index is out of bounds
 * 
 * @warning The returned pointer becomes invalid if the array is resized or destroyed
 * @warning Modifying the element size can corrupt the array structure
 * @note This function provides zero-copy access to array elements
 */
void *sat_array_get_reference_by (const sat_array_t *const object, const uint32_t index);

#endif/* SAT_ARRAY_H_ */
