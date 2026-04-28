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

/**
 * @brief Continue to next iteration when result is false
 *
 * Convenience macro that checks a boolean expression and continues to the
 * next loop iteration when the result is false. Useful for filtering items
 * in search loops without adding extra nesting.
 *
 * @param result Boolean expression to evaluate
 */
#define sat_status_continue_on_false(result) \
    if (result == false) \
    { \
        continue; \
    }

/**
 * @brief Break if value equals expected
 *
 * Convenience macro that compares a value against an expected value,
 * sets failure status with the given message, and breaks out of the loop
 * if they are equal.
 *
 * @param status Status object to set on failure
 * @param value Value to check
 * @param expected Expected value to compare against
 * @param message Error message string literal
 */
#define sat_status_break_if_equals(status, value, expected, message) \
    if (value == expected) \
    { \
        sat_status_failure (&status, "" message ""); \
        break; \
    }

/**
 * @brief Break if value not equals expected
 * 
 * Convenience macro that compares a value against an expected value,
 * sets failure status with the given message, and breaks out of the loop
 * if they are not equal.
 * 
 * @param status Status object to set on failure
 * @param value Value to check
 * @param expected Expected value to compare against
 * @param message Error message string literal
 */
#define sat_status_break_if_not_equals(status, value, expected, message) \
    if (value != expected) \
    { \
        sat_status_failure (&status, "" message ""); \
        break; \
    }

/**
 * @brief Break if value is NULL
 * 
 * Convenience macro that checks if a pointer is NULL, sets failure status
 * with the given message, and breaks out of the loop if so.
 * 
 * @param status Status object to set on failure
 * @param value Pointer to check
 * @param message Error message string literal
 */
#define sat_status_break_if_null(status, value, message) \
    if (value == NULL) \
    { \
        sat_status_failure (&status, "" message ""); \
        break; \
    }

/**
 * @brief Break if value is false
 * 
 * Convenience macro that checks if a boolean value is false, sets failure
 * status with the given message, and breaks out of the loop if so.
 * 
 * @param status Status object to set on failure
 * @param value Boolean value to check
 * @param message Error message string literal
 */
#define sat_status_break_if_false(status, value, message) \
    if (value == false) \
    { \
        sat_status_failure (&status, "" message ""); \
        break; \
    }

/**
 * @brief Return from function on error status
 * 
 * Convenience macro that evaluates a status-returning expression and returns
 * from the current function if the result indicates failure. The error status
 * is propagated to the caller.
 * 
 * @param value Expression that evaluates to sat_status_t
 */
#define sat_status_return_on_error(value) \
    {\
        sat_status_t status = value; \
        if (sat_status_get_result (&status) == false) \
        { \
            return status; \
        }\
    }

/**
 * @brief Return failure if value equals expected
 * 
 * Convenience macro that compares a value against an expected value and
 * returns a failure status with the given message if they are equal.
 * 
 * @param value Value to check
 * @param expected Expected value to compare against
 * @param message Error message string literal
 */
#define sat_status_return_on_equals(value, expected, message) \
    if (value == expected) \
    { \
        return sat_status_failure (&(sat_status_t){}, "" message ""); \
    }

/**
 * @brief Return failure if value not equals expected
 * 
 * Convenience macro that compares a value against an expected value and
 * returns a failure status with the given message if they are not equal.
 * The returned status stores the current function name in the where field.
 * 
 * @param value Value to check
 * @param expected Expected value to compare against
 * @param message Error message string literal
 */
#define sat_status_return_on_not_equals(value, expected, message) \
    if (value != expected) \
    { \
        return sat_status_failure_where (&(sat_status_t){}, __func__, "" message ""); \
    }

/**
 * @brief Return failure if value is NULL
 * 
 * Convenience macro that checks if a pointer is NULL and returns a failure
 * status with the given message if so. The returned status stores the
 * current function name in the where field.
 * 
 * @param value Pointer to check
 * @param message Error message string literal
 */
#define sat_status_return_on_null(value, message) \
    if (value == NULL) \
    { \
        return sat_status_failure_where (&(sat_status_t){}, __func__, "" message ""); \
    }

/**
 * @brief Return failure if value is false
 * 
 * Convenience macro that checks if a boolean value is false and returns
 * a failure status with the given message if so. The returned status stores
 * the current function name in the where field.
 * 
 * @param value Boolean value to check
 * @param message Error message string literal
 */
#define sat_status_return_on_false(value, message) \
    if (value == false) \
    { \
        return sat_status_failure_where (&(sat_status_t){}, __func__, "" message ""); \
    }

/**
 * @brief Return failure if value is greater than expected
 * 
 * Convenience macro that compares a value against an expected value and
 * returns a failure status with the given message if value > expected.
 * The returned status stores the current function name in the where field.
 * 
 * @param value Value to check
 * @param expected Expected value to compare against
 * @param message Error message string literal
 */
#define sat_status_return_on_greater_than(value, expected, message) \
    if (value > expected) \
    { \
        return sat_status_failure_where (&(sat_status_t){}, __func__, "" message ""); \
    }

/**
 * @brief Return failure if value is less than expected
 * 
 * Convenience macro that compares a value against an expected value and
 * returns a failure status with the given message if value < expected.
 * The returned status stores the current function name in the where field.
 * 
 * @param value Value to check
 * @param expected Expected value to compare against
 * @param message Error message string literal
 */
#define sat_status_return_on_less_than(value, expected, message) \
    if (value < expected) \
    { \
        return sat_status_failure_where (&(sat_status_t){}, __func__, "" message ""); \
    }

/**
 * @brief Return failure if value is greater than or equal to expected
 * 
 * Convenience macro that compares a value against an expected value and
 * returns a failure status with the given message if value >= expected.
 * The returned status stores the current function name in the where field.
 * 
 * @param value Value to check
 * @param expected Expected value to compare against
 * @param message Error message string literal
 */
#define sat_status_return_on_greater_than_or_equal(value, expected, message) \
    if (value >= expected) \
    { \
        return sat_status_failure_where (&(sat_status_t){}, __func__, "" message ""); \
    }

/**
 * @brief Return failure if value is less than or equal to expected
 * 
 * Convenience macro that compares a value against an expected value and
 * returns a failure status with the given message if value <= expected.
 * The returned status stores the current function name in the where field.
 * 
 * @param value Value to check
 * @param expected Expected value to compare against
 * @param message Error message string literal
 */
#define sat_status_return_on_less_than_or_equal(value, expected, message) \
    if (value <= expected) \
    { \
        return sat_status_failure_where (&(sat_status_t){}, __func__, "" message ""); \
    }

/**
 * @brief Return success status from current function
 * 
 * Convenience macro that creates and returns a success status in one step.
 * Useful for simplifying return statements at the end of successful functions.
 */
#define sat_status_return_on_success() \
    return sat_status_success (&(sat_status_t){})

/**
 * @brief Return failure status with message from current function
 * 
 * Convenience macro that creates and returns a failure status with the given
 * error message in one step. The returned status stores the current
 * function name in the where field.
 * 
 * @param message Error message string literal
 */
#define sat_status_return_on_failure(message) \
    return sat_status_failure_where (&(sat_status_t){}, __func__, "" message "")

/**
 * @brief Terminate program with error message
 * 
 * Convenience macro that calls sat_status_abort() to terminate the program
 * immediately with an error message.
 * 
 * @param message Error message string literal
 */
#define sat_status_abort_on_error(message) \
    sat_status_abort (&(sat_status_t){}, "" message "")

/**
 * @brief Status structure with result, source, and error message
 * 
 * Represents the outcome of an operation with a boolean result
 * and optional descriptive metadata about the error source and motive.
 */
typedef struct 
{
    bool result;           /**< Success (true) or failure (false) */
    const char *motive;    /**< Error description or "No error" */
    const char *where;     /**< Source function or context where status was created */
} sat_status_t;

/**
 * @brief Set status result and error message
 * 
 * Configures a status object with the specified result, source location,
 * and error message. If where is NULL or empty, it defaults to an empty
 * string. If the motive is NULL or empty, it defaults to "No error".
 * 
 * @param object Pointer to status object to configure
 * @param result Success (true) or failure (false)
 * @param where Source location string (can be NULL)
 * @param motive Error description string (can be NULL)
 * @return The configured status object by value
 */
sat_status_t sat_status_set (sat_status_t *const object, bool result, const char *const where, const char *const motive);

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
 * @brief Get the source location from a status object
 * 
 * Retrieves the source context stored in a status object. This is typically
 * the current function name passed with __func__ when the status was created.
 * For statuses created without an explicit source, this returns an empty string.
 * 
 * @param object Pointer to status object
 * @return Pointer to source location string
 */
const char *sat_status_get_where (const sat_status_t *const object);

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

/**
 * @brief Create a failure status with source location and error message
 * 
 * Convenience function that creates a status indicating failed operation
 * while preserving the source context where the error originated.
 * 
 * @param object Pointer to status object to initialize
 * @param where Source location string, usually __func__
 * @param motive Error description string
 * @return Failure status by value
 */
sat_status_t sat_status_failure_where (sat_status_t *const object, const char *const where, const char *const motive);

/**
 * @brief Terminate program execution with error message
 * 
 * Prints an error message to stderr and immediately terminates the program
 * using abort(). This function is intended for irrecoverable errors where
 * the program cannot continue safely.
 * 
 * @param object Pointer to status object (for consistency with other functions)
 * @param message Error message to display before terminating
 * 
 * @warning This function does not return - it terminates the program
 * @note Use this only for critical errors where recovery is impossible
 */
void sat_status_abort (sat_status_t *const object, const char *const message);

#endif/* SAT_STATUS_H */
