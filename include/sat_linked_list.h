#ifndef SAT_LINKED_LIST_H_
#define SAT_LINKED_LIST_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct sat_linked_list_t sat_linked_list_t;

typedef bool (*sat_linked_list_compare_t) (const void *const element, const void *const data);
typedef void  (*sat_linked_list_print_t) (const void *const element);

/**
 * @brief Creates a new linked list object.
 * 
 * @param object Pointer to the linked list object pointer.
 * @param object_size Size of the objects to be stored in the linked list.
 * @return sat_status_t Status of the operation.
 */
sat_status_t sat_linked_list_create (sat_linked_list_t **const object, const uint32_t object_size);

/**
 * @brief Inserts a new element into the linked list.
 * 
 * @param object Pointer to the linked list object.
 * @param element Pointer to the element to be inserted.
 * @return sat_status_t Status of the operation.
 */
sat_status_t sat_linked_list_insert (sat_linked_list_t *const object, const void *const element);

/**
 * @brief Removes an element from the linked list.
 * 
 * @param object Pointer to the linked list object.
 * @param compare Function pointer to the comparison function.
 * @param param Pointer to the parameter used for comparison.
 * @return sat_status_t Status of the operation.
 */
sat_status_t sat_linked_list_remove (sat_linked_list_t *const object, sat_linked_list_compare_t compare, const void *const param);

/**
 * @brief Retrieves an element from the linked list.
 * 
 * @param object Pointer to the linked list object.
 * @param compare Function pointer to the comparison function.
 * @param param Pointer to the parameter used for comparison.
 * @param element Pointer to the variable where the retrieved element will be stored.
 * @return sat_status_t Status of the operation.
 */
sat_status_t sat_linked_list_get (const sat_linked_list_t *const object, sat_linked_list_compare_t compare, const void *const param, void *const element);
/**
 * Checks if an element is present in the linked list by using a comparison function.
 *
 * @param object Pointer to the sat_linked_list_t object.
 * @param compare Function pointer to the comparison function.
 * @param param Pointer to the parameter used for comparison.
 * @return sat_status_t indicating whether the element is present or not.
 * 
 * @note If the user want to get the data, uses @see sat_linked_list_get
 */
sat_status_t sat_linked_list_is_present (const sat_linked_list_t *const object, sat_linked_list_compare_t compare, const void *const param);

/**
 * @brief Retrieves the size of the linked list.
 * 
 * @param object Pointer to the linked list object.
 * @param size Pointer to the variable where the size will be stored.
 * @return sat_status_t Status of the operation.
 */
sat_status_t sat_linked_list_get_size (const sat_linked_list_t *const object, uint32_t *const size);

/**
 * @brief Prints the elements of the linked list using a user-provided print function.
 * 
 * @param object Pointer to the linked list object.
 * @param print Function pointer to the print function.
 * @return sat_status_t Status of the operation.
 */
sat_status_t sat_linked_list_debug (const sat_linked_list_t *const object, sat_linked_list_print_t print);

/**
 * @brief Destroys the linked list object and frees associated resources.
 * 
 * @param object Pointer to the linked list object.
 * @return sat_status_t Status of the operation.
 */
sat_status_t sat_linked_list_destroy (sat_linked_list_t *const object);

#endif/* SAT_LINKED_LIST_H_ */
 