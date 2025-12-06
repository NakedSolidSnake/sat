#ifndef SAT_ARRAY_H_
#define SAT_ARRAY_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct sat_array_t sat_array_t;

typedef bool (*sat_array_compare_t) (const void *const element, const void *const param);

typedef void (*sat_array_memory_notify_t) (void *const user, uint32_t new_size);

typedef enum
{
    sat_array_mode_static,
    sat_array_mode_dynamic,
} sat_array_mode_t;

typedef struct
{
    uint32_t size;
    uint32_t object_size;
    sat_array_mode_t mode;
    
    struct
    {
        sat_array_memory_notify_t on_increase;
        void *user;
    } notification;

} sat_array_args_t;

/**
 * Creates a new sat_array_t object.
 *
 * @param object Pointer to the pointer that will hold the created sat_array_t object.
 * @param args Pointer to the sat_array_args_t structure containing the array configuration.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_create (sat_array_t **const object, const sat_array_args_t *const args);

/**
 * @brief Adds an object to the array.
 * 
 * @param object Pointer to the sat_array_t object.
 * @param data Pointer to the data to be added.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_add (sat_array_t *const object, const void *const data);

/**
 * @brief Updates an object in the array at the specified index.
 * 
 * @param object Pointer to the sat_array_t object.
 * @param data Pointer to the new data to update.
 * @param index Index of the object to update.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_update_by (sat_array_t *const object, const void *const data, const uint32_t index);

/**
 * @brief Removes an object from the array at the specified index.
 * 
 * @param object Pointer to the sat_array_t object.
 * @param index Index of the object to remove.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_remove_by (sat_array_t *const object, const uint32_t index);

/**
 * Removes an object from the array by matching a parameter using a comparison function.
 *
 * @param object Pointer to the sat_array_t object.
 * @param param Pointer to the parameter used for comparison.
 * @param compare Function pointer to the comparison function.
 * @param data Pointer to store the removed object's data. If is null, the data will be discarted.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_remove_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare ,void *const data);

/**
 * @brief Retrieves an object from the array at the specified index.
 * 
 * @param object Pointer to the sat_array_t object.
 * @param index Index of the object to retrieve.
 * @param data Pointer to store the retrieved object's data.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_get_object_by (const sat_array_t *const object, const uint32_t index, void *const data);

/**
 * @brief Retrieves an object from the array by matching a parameter using a comparison function.
 * 
 * @param object Pointer to the sat_array_t object.
 * @param param Pointer to the parameter used for comparison.
 * @param compare Function pointer to the comparison function.
 * @param data Pointer to store the retrieved object's data.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_get_object_by_parameter (sat_array_t *const object, const void *const param, sat_array_compare_t compare, void *const data);

/**
 * @brief Retrieves the size of the array.
 * 
 * @param object Pointer to the sat_array_t object.
 * @param size Pointer to store the size of the array.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_get_size (const sat_array_t *const object, uint32_t *const size);

/**
 * @brief Clears all objects from the array.
 * 
 * @param object Pointer to the sat_array_t object.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_clear (sat_array_t *const object);

/**
 * @brief Retrieves the capacity of the array.
 * 
 * @param object Pointer to the sat_array_t object.
 * @param capacity Pointer to store the capacity of the array.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_get_capacity (const sat_array_t *const object, uint32_t *const capacity);

/**
 * @brief Destroys the array and frees associated resources.
 * 
 * @param object Pointer to the sat_array_t object.
 * @return sat_status_t indicating success or failure of the operation.
 */
sat_status_t sat_array_destroy (sat_array_t *const object);

void *sat_array_get_reference_by (const sat_array_t *const object, const uint32_t index);

#endif/* SAT_ARRAY_H_ */
