/**
 * @file sat_status.h
 * @brief Status handling and error reporting system
 * 
 * This module provides a structured way to handle function return status
 * with boolean success indicators and descriptive error messages. It includes
 * convenience macros for control flow based on status results.
 * 
 * The status system is designed to be lightweight and easy to use, providing
 * clear success/failure indication with human-readable error descriptions.
 */

#ifndef SAT_STATUS_H
#define SAT_STATUS_H

#include <stdbool.h>

/**
 * @brief Break out of loop on error status
 * 
 * Convenience macro that checks if a status indicates failure and breaks
 * out of the current loop if so. Useful in do-while(false) error handling
 * patterns.
 * 
 * @param status Status object to check
 */
#define sat_status_break_on_error(status) \
    if (sat_status_get_result (&status) == false) \
    { \
        break; \
    }

#define sat_status_return_on_error(value) \
    {\
        sat_status_t status = value; \
        if (sat_status_get_result (&status) == false) \
        { \
            return status; \
        }\
    }

/**
 * @brief Continue to next iteration on error status
 * 
 * Convenience macro that checks if a status indicates failure and continues
 * to the next loop iteration if so. Useful for processing multiple items
 * where individual failures should not stop the entire operation.
 * 
 * @param status Status object to check
 */
#define sat_status_continue_on_error(status) \
    if (sat_status_get_result (&status) == false) \
    { \
        continue; \
    }

#define sat_status_break_if_equals(status, value, expected, message) \
    if (value == expected) \
    { \
        sat_status_failure (&status, "" message ""); \
        break; \
    }

#define sat_status_break_if_not_equals(status, value, expected, message) \
    if (value != expected) \
    { \
        sat_status_failure (&status, "" message ""); \
        break; \
    }

#define sat_status_break_if_null(status, value, message) \
    if (value == NULL) \
    { \
        sat_status_failure (&status, "" message ""); \
        break; \
    }

#define sat_status_break_if_false(status, value, message) \
    if (value == false) \
    { \
        sat_status_failure (&status, "" message ""); \
        break; \
    }

#define sat_status_return_if_equals(value, expected, message) \
    if (value == expected) \
    { \
        return sat_status_failure (&(sat_status_t){}, "" message ""); \
    }

#define sat_status_return_if_not_equals(value, expected, message) \
    if (value != expected) \
    { \
        return sat_status_failure (&(sat_status_t){}, "" message ""); \
    }

#define sat_status_return_if_null(value, message) \
    if (value == NULL) \
    { \
        return sat_status_failure (&(sat_status_t){}, "" message ""); \
    }

#define sat_status_return_if_false(value, message) \
    if (value == false) \
    { \
        return sat_status_failure (&(sat_status_t){}, "" message ""); \
    }

#define sat_status_return_greater_than(value, expected, message) \
    if (value > expected) \
    { \
        return sat_status_failure (&(sat_status_t){}, "" message ""); \
    }

#define sat_status_return_less_than(value, expected, message) \
    if (value < expected) \
    { \
        return sat_status_failure (&(sat_status_t){}, "" message ""); \
    }

#define sat_status_return_greater_than_or_equal(value, expected, message) \
    if (value >= expected) \
    { \
        return sat_status_failure (&(sat_status_t){}, "" message ""); \
    }

#define sat_status_return_less_than_or_equal(value, expected, message) \
    if (value <= expected) \
    { \
        return sat_status_failure (&(sat_status_t){}, "" message ""); \
    }

#define sat_status_return_success() \
    return sat_status_success (&(sat_status_t){})

#define sat_status_return_failure(message) \
    return sat_status_failure (&(sat_status_t){}, "" message "")


#define sat_status_abort_on_error(message) \
    sat_status_abort (&(sat_status_t){}, "" message "")

/**
 * @brief Status structure with result and error message
 * 
 * Represents the outcome of an operation with a boolean result
 * and an optional descriptive error message.
 */
typedef struct 
{
    bool result;     /**< Success (true) or failure (false) */
    const char *motive;    /**< Error description or "No error" */
} sat_status_t;

/**
 * @brief Set status result and error message
 * 
 * Configures a status object with the specified result and error message.
 * If the motive is NULL or empty, it defaults to "No error".
 * 
 * @param object Pointer to status object to configure
 * @param result Success (true) or failure (false)
 * @param motive Error description string (can be NULL)
 * @return The configured status object by value
 */
sat_status_t sat_status_set (sat_status_t *const object, bool result, const char *const motive);

/**
 * @brief Get the result from a status object
 * 
 * Retrieves the boolean success/failure indicator from a status object.
 * 
 * @param object Pointer to status object
 * @return true for success, false for failure
 */
bool sat_status_get_result (const sat_status_t *const object);

/**
 * @brief Get the error message from a status object
 * 
 * Retrieves the descriptive error message from a status object.
 * Returns "No error" for successful operations.
 * 
 * @param object Pointer to status object
 * @return Pointer to error message string
 */
const char *sat_status_get_motive (const sat_status_t *const object);

/**
 * @brief Create a success status
 * 
 * Convenience function that creates a status indicating successful
 * operation with no error message.
 * 
 * @param object Pointer to status object to initialize
 * @return Success status by value
 */
sat_status_t sat_status_success (sat_status_t *const object);

/**
 * @brief Create a failure status with error message
 * 
 * Convenience function that creates a status indicating failed
 * operation with a descriptive error message.
 * 
 * @param object Pointer to status object to initialize
 * @param motive Error description string
 * @return Failure status by value
 */
sat_status_t sat_status_failure (sat_status_t *const object, const char *const motive);


void sat_status_abort (sat_status_t *const object, const char *const message);

#endif/* SAT_STATUS_H */
