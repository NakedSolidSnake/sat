/**
 * @file sat_validation.h
 * @brief Data validation framework with composable criteria
 * @author SAT Library Team
 * @date December 2025
 * 
 * This module provides a flexible validation framework that allows defining
 * and composing multiple validation criteria. It supports building complex
 * validation rules by combining individual criterion functions, making it
 * ideal for input validation, data integrity checking, and business rule
 * enforcement.
 * 
 * The framework uses a callback-based approach where each criterion is a
 * function that validates a specific aspect of the data. All criteria must
 * pass for the validation to succeed.
 */

#ifndef SAT_VALIDATION_H
#define SAT_VALIDATION_H

#include <stdint.h>
#include <sat_status.h>

/** Maximum number of criteria that can be registered in a validator */
#define SAT_VALIDATION_CRITERIA_AMOUNT      10

/**
 * @brief Validation criteria function pointer type
 * 
 * This function type defines the signature for validation criteria. Each
 * criterion receives a pointer to data to validate and returns a status
 * indicating whether the criterion is satisfied.
 * 
 * @param data Pointer to the data to validate (type depends on context)
 * @return Status indicating success (valid) or failure (invalid) with description
 * 
 * @note Criteria functions should not modify the data
 * @note Multiple criteria can be combined to form complex validation rules
 */
typedef sat_status_t (*sat_criteria_t) (const void *const data);

/**
 * @brief Validation object containing registered criteria
 * 
 * This structure holds a collection of validation criteria that will be
 * executed in sequence. All criteria must pass for the validation to succeed.
 */
typedef struct 
{
    sat_criteria_t criterias [SAT_VALIDATION_CRITERIA_AMOUNT];  /**< Array of criteria functions */
    uint8_t amount;                                             /**< Number of registered criteria */
} sat_validation_t;

/**
 * @brief Initialize a validation object
 * 
 * Prepares a validation object for use by clearing all fields. This must be
 * called before adding criteria or performing validation.
 * 
 * @param[out] object Pointer to validation object to initialize
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Successfully initialized
 * @retval SAT_STATUS_ERROR Failed (null object)
 * 
 * @see sat_validation_add(), sat_validation_verify()
 */
sat_status_t sat_validation_init (sat_validation_t *const object);

/**
 * @brief Add a validation criterion to the validator
 * 
 * Registers a new criterion function with the validator. Criteria are executed
 * in the order they are added. The maximum number of criteria is limited by
 * SAT_VALIDATION_CRITERIA_AMOUNT.
 * 
 * @param[in,out] object Pointer to initialized validation object
 * @param[in] criteria Criterion function to add
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Criterion successfully added
 * @retval SAT_STATUS_ERROR Failed (null object/criteria or maximum criteria reached)
 * 
 * @note Criteria are evaluated in the order they are added
 * @note Maximum of SAT_VALIDATION_CRITERIA_AMOUNT criteria can be added
 * @see sat_validation_init(), sat_validation_verify()
 */
sat_status_t sat_validation_add (sat_validation_t *const object, sat_criteria_t criteria);

/**
 * @brief Verify data against all registered criteria
 * 
 * Executes all registered criteria functions against the provided data. Each
 * criterion is evaluated in sequence, and validation stops at the first failure.
 * All criteria must pass for the validation to succeed.
 * 
 * @param[in] object Pointer to validation object with registered criteria
 * @param[in] data Pointer to data to validate (interpretation depends on criteria)
 * @return Status indicating success (all criteria passed) or failure (at least one failed)
 * @retval SAT_STATUS_OK All criteria passed
 * @retval SAT_STATUS_ERROR At least one criterion failed or invalid parameters
 * 
 * @note Validation stops at the first failing criterion
 * @note The status from the first failing criterion is returned
 * @note Data is passed as void* to allow validating any data type
 * @see sat_validation_add()
 */
sat_status_t sat_validation_verify (const sat_validation_t *const object, const void *const data);

#endif/* SAT_VALIDATION_H */
