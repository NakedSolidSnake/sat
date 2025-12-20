/**
 * @file sat_log.h
 * @brief Flexible logging system with multiple levels and custom loggers
 * @author SAT Library Team
 * @date December 2025
 * 
 * This module provides a comprehensive logging system with multiple severity
 * levels, custom logger support, and convenient macros for quick logging.
 * It supports both simple macro-based logging and structured logging with
 * custom output handlers.
 * 
 * The module offers two logging approaches:
 * 1. Quick macros (sat_log_error, sat_log_warn, sat_log_info, sat_log_debug)
 * 2. Structured logging with sat_log_t objects and custom handlers
 */

#ifndef SAT_LOG_H
#define SAT_LOG_H

#include <sat_status.h>
#include <stdarg.h>
#include <stdio.h>

/**
 * @brief Global variable controlling macro-based logging levels
 * 
 * Controls which log macros produce output:
 * - 0: No logging
 * - 1: ERROR only
 * - 2: ERROR, WARN
 * - 3: ERROR, WARN, INFO
 * - 4: ERROR, WARN, INFO, DEBUG
 * 
 * Can be set via SAT_LOG environment variable or directly in code.
 */
extern int sat_log_enabled;

/**
 * @brief Log an error message with file location
 * 
 * Logs an error message to stderr with line number and function name.
 * Only outputs if sat_log_enabled >= 1.
 * 
 * @param fmt Printf-style format string
 * @param ... Variable arguments for format string
 * 
 * @note Uses stderr for output
 * @note Includes line number and function name automatically
 */
#define sat_log_error(fmt, ...) \
    do { if (sat_log_enabled >= 1) fprintf (stderr, "[ERROR] %d:%s(): " fmt "\n", __LINE__, __func__, ##__VA_ARGS__); } while (0)
    
/**
 * @brief Log a warning message with file location
 * 
 * Logs a warning message to stderr with line number and function name.
 * Only outputs if sat_log_enabled >= 2.
 * 
 * @param fmt Printf-style format string
 * @param ... Variable arguments for format string
 * 
 * @note Uses stderr for output
 * @note Includes line number and function name automatically
 */
#define sat_log_warn(fmt, ...) \
    do { if (sat_log_enabled >= 2) fprintf (stderr, "[WARN] %d:%s(): " fmt "\n", __LINE__, __func__, ##__VA_ARGS__); } while (0)

/**
 * @brief Log an informational message
 * 
 * Logs an informational message to stdout. Only outputs if sat_log_enabled >= 3.
 * 
 * @param fmt Printf-style format string
 * @param ... Variable arguments for format string
 * 
 * @note Uses stdout for output
 * @note Does not include file location information
 */
#define sat_log_info(fmt, ...) \
    do { if (sat_log_enabled >= 3) fprintf (stdout, "[INFO] " fmt "\n", ##__VA_ARGS__); } while (0)

/**
 * @brief Log a debug message with file location
 * 
 * Logs a debug message to stderr with line number and function name.
 * Only outputs if sat_log_enabled >= 4.
 * 
 * @param fmt Printf-style format string
 * @param ... Variable arguments for format string
 * 
 * @note Uses stderr for output
 * @note Includes line number and function name automatically
 */
#define sat_log_debug(fmt, ...) \
    do { if (sat_log_enabled >= 4) fprintf (stderr, "[DEBUG] %d:%s(): " fmt "\n", __LINE__, __func__, ##__VA_ARGS__); } while (0)

/**
 * @brief Logger type enumeration
 * 
 * Defines the type of logger implementation to use.
 */
typedef enum 
{
  sat_log_type_none,     /**< No logging - all messages discarded */
  sat_log_type_default,  /**< Default logger - uses printf */
  sat_log_type_custom    /**< Custom logger - user-provided function */
} sat_log_type_t;

/**
 * @brief Log level enumeration
 * 
 * Defines severity levels for structured logging, from most to least severe.
 */
typedef enum 
{
    sat_log_level_fatal,        /**< Fatal error - application cannot continue */
    sat_log_level_error,        /**< Error condition */
    sat_log_level_warning,      /**< Warning condition */
    sat_log_level_information,  /**< Informational message */
    sat_log_level_debug,        /**< Debug information */
    sat_log_level_trace         /**< Detailed trace information */
} sat_log_level_t;

/**
 * @brief Custom logger function pointer type
 * 
 * Function signature for custom logger implementations. The function receives
 * the log level, format string, and variable argument list for processing.
 * 
 * @param level Severity level of the message
 * @param format Printf-style format string
 * @param list Variable argument list for format string
 * 
 * @note Implementations should handle va_list appropriately
 * @note Should not modify the format string
 */
typedef void (*sat_log_logger_t) (sat_log_level_t level, const char *format, va_list list);

/**
 * @brief Log object structure
 * 
 * Represents a configured logger instance with its type and handler function.
 */
typedef struct 
{
    sat_log_type_t type;       /**< Logger type */
    sat_log_logger_t logger;   /**< Logger function pointer */
} sat_log_t;

/**
 * @brief Configuration parameters for logger initialization
 * 
 * Specifies the logger type and custom handler for creating a logger instance.
 */
typedef struct 
{
    sat_log_type_t type;       /**< Logger type to create */
    sat_log_logger_t logger;   /**< Custom logger function (only for custom type) */
} sat_log_args_t;

/**
 * @brief Initialize a logger object
 * 
 * Prepares a logger object for configuration. Must be called before
 * sat_log_open().
 * 
 * @param[out] object Pointer to logger object to initialize
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Successfully initialized
 * @retval SAT_STATUS_ERROR Failed (null object)
 * 
 * @see sat_log_open()
 */
sat_status_t sat_log_init (sat_log_t *object);

/**
 * @brief Open and configure a logger
 * 
 * Configures the logger with the specified type and handler. For custom
 * loggers, the logger function must be provided in args.
 * 
 * @param[in,out] object Pointer to initialized logger object
 * @param[in] args Configuration parameters
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Logger successfully configured
 * @retval SAT_STATUS_ERROR Failed (null object/args or invalid type)
 * 
 * @note For custom type, args->logger must not be NULL
 * @see sat_log_init(), sat_log_registry()
 */
sat_status_t sat_log_open (sat_log_t *object, sat_log_args_t *args);

/**
 * @brief Log a message through the logger
 * 
 * Sends a log message with the specified level through the configured logger.
 * The message is formatted using printf-style format strings.
 * 
 * @param[in] object Pointer to opened logger object
 * @param[in] level Severity level of the message
 * @param[in] format Printf-style format string
 * @param[in] ... Variable arguments for format string
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Message logged successfully
 * @retval SAT_STATUS_ERROR Failed (null object/format)
 * 
 * @note The actual output depends on the logger type
 * @see sat_log_open()
 */
sat_status_t sat_log_registry (sat_log_t *object, sat_log_level_t level, const char *format, ...);

/**
 * @brief Close and cleanup a logger
 * 
 * Closes the logger and releases resources. After calling this, the logger
 * object is no longer valid.
 * 
 * @param[in,out] object Pointer to logger object to close
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Logger successfully closed
 * @retval SAT_STATUS_ERROR Failed (null object)
 * 
 * @see sat_log_open()
 */
sat_status_t sat_log_close (sat_log_t *object);

/**
 * @brief Initialize logging from SAT_LOG environment variable
 * 
 * Configures sat_log_enabled based on the SAT_LOG environment variable:
 * - "debug": Enable all levels (sat_log_enabled = 4)
 * - "info": Enable info, warn, error (sat_log_enabled = 3)
 * - "warn": Enable warn, error (sat_log_enabled = 2)
 * - Other values: Enable error only (sat_log_enabled = 1)
 * - Not set: Use fallback value
 * 
 * @param[in] fallback Default logging level if SAT_LOG is not set
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Environment variable processed
 * @retval SAT_STATUS_ERROR Environment variable not set
 * 
 * @note Call this at application startup to configure macro-based logging
 */
sat_status_t sat_log_init_by_env (const char *const fallback);

#endif/* SAT_LOG_H */
