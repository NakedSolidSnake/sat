/**
 * @file sat_linked_list.h
 * @brief Doubly-linked list data structure
 * @author SAT Library Contributors
 * @date 2025
 * 
 * This module provides a doubly-linked list implementation that can store
 * arbitrary data types. It supports insertion, removal, searching, and
 * iteration operations. The list is compatible with the sat_iterator interface.
 */

#ifndef SAT_LINKED_LIST_H_
#define SAT_LINKED_LIST_H_

#include <sat_status.h>
#include <stdint.h>

/**
 * @brief Opaque structure representing a linked list
 * 
 * This structure holds the internal state of the linked list.
 * Users should not access its fields directly.
 */
typedef struct sat_linked_list_t sat_linked_list_t;

/**
 * @brief Comparison function type for searching elements
 * 
 * @param element Pointer to the list element being compared
 * @param data Pointer to the search parameter
 * @return true if the element matches the parameter, false otherwise
 */
typedef bool (*sat_linked_list_compare_t) (const void *const element, const void *const data);

/**
 * @brief Callback function type for printing elements
 * 
 * Used by sat_linked_list_debug() to print each element in the list.
 * 
 * @param element Pointer to the list element to print
 */
typedef void  (*sat_linked_list_print_t) (const void *const element);

/**
 * @brief Creates a new linked list object
 * 
 * Allocates and initializes a new linked list that can store elements
 * of the specified size.
 * 
 * @param[out] object Pointer to the pointer that will hold the created list
 * @param[in] object_size Size in bytes of each element to be stored
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The caller is responsible for calling sat_linked_list_destroy() to free resources
 * @see sat_linked_list_destroy()
 */
sat_status_t sat_linked_list_create (sat_linked_list_t **const object, const uint32_t object_size);

/**
 * @brief Inserts a new element into the linked list
 * 
 * Adds a new element to the list by copying it. Elements are typically
 * inserted at the end of the list.
 * 
 * @param[in,out] object Pointer to the linked list object
 * @param[in] element Pointer to the element to be inserted
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The element is copied into the list using memcpy
 * @note Memory is automatically allocated for the new node
 */
sat_status_t sat_linked_list_insert (sat_linked_list_t *const object, const void *const element);

/**
 * @brief Removes an element from the linked list
 * 
 * Searches for and removes the first element matching the given parameter
 * using the provided comparison function.
 * 
 * @param[in,out] object Pointer to the linked list object
 * @param[in] compare Function pointer to the comparison function
 * @param[in] param Pointer to the parameter used for comparison
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Only the first matching element is removed
 * @note The memory for the removed node is freed
 * @see sat_linked_list_compare_t
 */
sat_status_t sat_linked_list_remove (sat_linked_list_t *const object, sat_linked_list_compare_t compare, const void *const param);

/**
 * @brief Retrieves an element from the linked list
 * 
 * Searches for the first element matching the given parameter using the
 * provided comparison function, then copies it to the output buffer.
 * 
 * @param[in] object Pointer to the linked list object
 * @param[in] compare Function pointer to the comparison function
 * @param[in] param Pointer to the parameter used for comparison
 * @param[out] element Pointer to buffer where the retrieved element will be copied
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning The element buffer must be large enough to hold object_size bytes
 * @note Returns the first matching element
 * @note The list is not modified
 * @see sat_linked_list_compare_t
 */
sat_status_t sat_linked_list_get (const sat_linked_list_t *const object, sat_linked_list_compare_t compare, const void *const param, void *const element);

/**
 * @brief Gets a direct reference to an element in the linked list
 * 
 * Searches for the first element matching the given parameter using the
 * provided comparison function, then returns a pointer to it without copying.
 * 
 * @param[in] object Pointer to the linked list object
 * @param[in] compare Function pointer to the comparison function
 * @param[in] param Pointer to the parameter used for comparison
 * @param[out] element Pointer to store the reference to the matched element
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning The returned pointer becomes invalid if the element is removed or the list is destroyed
 * @warning Modifying the element size can corrupt the list structure
 * @note This function provides zero-copy access to list elements
 * @see sat_linked_list_get()
 * @see sat_linked_list_compare_t
 */
sat_status_t sat_linked_list_get_ref (const sat_linked_list_t *const object, sat_linked_list_compare_t compare, const void *const param, void **const element);

/**
 * @brief Checks if an element is present in the linked list
 * 
 * Searches for an element matching the given parameter using the provided
 * comparison function without retrieving it.
 *
 * @param[in] object Pointer to the linked list object
 * @param[in] compare Function pointer to the comparison function
 * @param[in] param Pointer to the parameter used for comparison
 * @return sat_status_t indicating whether the element is present or not
 * 
 * @note If you need to retrieve the data, use sat_linked_list_get() instead
 * @see sat_linked_list_get()
 * @see sat_linked_list_compare_t
 */
sat_status_t sat_linked_list_is_present (const sat_linked_list_t *const object, sat_linked_list_compare_t compare, const void *const param);

/**
 * @brief Retrieves the size of the linked list
 * 
 * Returns the number of elements currently stored in the list.
 * 
 * @param[in] object Pointer to the linked list object
 * @param[out] size Pointer to store the number of elements
 * @return sat_status_t indicating success or failure of the operation
 */
sat_status_t sat_linked_list_get_size (const sat_linked_list_t *const object, uint32_t *const size);

/**
 * @brief Prints the elements of the linked list for debugging
 * 
 * Traverses the list and calls the provided print function for each element.
 * The list is not modified.
 * 
 * @param[in] object Pointer to the linked list object
 * @param[in] print Function pointer to the print function
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The print function receives a pointer to each element
 * @see sat_linked_list_print_t
 */
sat_status_t sat_linked_list_debug (const sat_linked_list_t *const object, sat_linked_list_print_t print);

/**
 * @brief Destroys the linked list and frees all associated resources
 * 
 * Deallocates all memory used by the list, including all nodes and elements.
 * 
 * @param[in,out] object Pointer to the linked list object
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note After calling this function, the list pointer becomes invalid
 * @warning Calling any list functions on a destroyed list results in undefined behavior
 * @see sat_linked_list_create()
 */
sat_status_t sat_linked_list_destroy (sat_linked_list_t *const object);

#endif/* SAT_LINKED_LIST_H_ */
 