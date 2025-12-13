#ifndef SAT_SET_H_
#define SAT_SET_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct sat_set_t sat_set_t;

typedef bool (*sat_set_compare_t) (const void *const element, const void *const param);
typedef bool (*sat_set_is_equal_t) (const void *const element, const void *const new_element);

typedef enum
{
    sat_set_mode_static,
    sat_set_mode_dynamic,
} sat_set_mode_t;

typedef struct
{
    uint32_t size;
    uint32_t object_size;
    sat_set_is_equal_t is_equal;
    sat_set_mode_t mode;
} sat_set_args_t;

/**
 * @brief Create a set object
 * 
 * @param object Pointer to the set object pointer
 * @param args Pointer to the set arguments
 * @return sat_status_t Status of the operation
 */
sat_status_t sat_set_create (sat_set_t **const object, const sat_set_args_t *args);

/**
 * @brief Add an element to the set
 * 
 * @param object Pointer to the set object
 * @param data Pointer to the data to add
 * @return sat_status_t Status of the operation
 */
sat_status_t sat_set_add (sat_set_t *const object, const void *const data);

/**
 * @brief Update an element in the set by index
 * 
 * @param object Pointer to the set object
 * @param data Pointer to the new data
 * @param index Index of the element to update
 * @return sat_status_t Status of the operation
 */
sat_status_t sat_set_update_by (sat_set_t *const object, const void *const data, uint32_t index);

/**
 * @brief Remove an element from the set by index
 * 
 * @param object Pointer to the set object
 * @param index Index of the element to remove
 * @return sat_status_t Status of the operation
 */
sat_status_t sat_set_remove_by (sat_set_t *const object, uint32_t index);

/**
 * @brief Remove an element from the set by parameter
 * 
 * @param object Pointer to the set object
 * @param param Pointer to the parameter to compare
 * @param compare Comparison function
 * @param data Pointer to the data to remove
 * @return sat_status_t Status of the operation
 */
sat_status_t sat_set_remove_by_parameter (sat_set_t *const object, const void *const param, sat_set_compare_t compare, void *const data);

/**
 * @brief Get an element from the set by index
 * 
 * @param object Pointer to the set object
 * @param index Index of the element to get
 * @param data Pointer to the data to store the element
 * @return sat_status_t Status of the operation
 */
sat_status_t sat_set_get_object_by (const sat_set_t *const object, uint32_t index, void *const data);

/**
 * @brief Get an element from the set by parameter
 * 
 * @param object Pointer to the set object
 * @param param Pointer to the parameter to compare
 * @param compare Comparison function
 * @param data Pointer to the data to store the element
 * @return sat_status_t Status of the operation
 */
sat_status_t sat_set_get_object_by_parameter (const sat_set_t *const object, const void *const param, sat_set_compare_t compare, void *const data);


/**
 * @brief  Get a reference to an element from the set by parameter
 * 
 * @param object Pointer to the set object
 * @param param Pointer to the parameter to compare
 * @param compare Comparison function
 * @param data Pointer to the pointer to store the reference to the element
 * @return sat_status_t Status of the operation
 */
sat_status_t sat_set_get_object_ref_by_parameter (sat_set_t *const object, const void *const param, sat_set_compare_t compare, void **const data);

/**
 * @brief Get the size of the set
 * 
 * @param object Pointer to the set object
 * @param size Pointer to store the size of the set
 * @return sat_status_t Status of the operation
 */
sat_status_t sat_set_get_size (const sat_set_t *const object, uint32_t *const size);

/**
 * @brief Destroy the set object
 * 
 * @param object Pointer to the set object
 * @return sat_status_t Status of the operation
 */
sat_status_t sat_set_destroy (sat_set_t *const object);

#endif/* SAT_SET_H_ */
