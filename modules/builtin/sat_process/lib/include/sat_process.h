/**
 * @file sat_process.h
 * @brief Process management and spawning utilities
 * @author SAT Library Team
 * @date December 2025
 * 
 * This module provides utilities for creating and spawning child processes with
 * configurable recovery modes. It supports process launching with or without
 * arguments and integrates with the sat_process_monitor for automatic recovery.
 */

#ifndef SAT_PROCESS_H_
#define SAT_PROCESS_H_

#include <unistd.h>
#include <sat_status.h>

/** Maximum size for process name */
#define SAT_PROCESS_NAME_SIZE   128

/** Maximum size for process arguments string */
#define SAT_PROCESS_ARGS_SIZE   1024

/**
 * @brief Process recovery mode
 * 
 * Defines how a process should be handled when it terminates unexpectedly.
 */
typedef enum 
{
    sat_process_mode_no_recover,  /**< Do not restart process on termination */
    sat_process_mode_recover,     /**< Restart process automatically on termination */
    sat_process_mode_critical     /**< Restart process; terminate monitor if restart fails */

} sat_process_mode_t;

/**
 * @brief Process spawn type indicating argument handling
 * 
 * Internal type used to determine how to spawn the process.
 */
typedef enum 
{
    sat_process_spawn_type_no_args,    /**< Spawn without arguments */
    sat_process_spawn_type_with_args,  /**< Spawn with arguments */

} sat_process_spawn_type_t;

/**
 * @brief Process structure containing process state and configuration
 * 
 * This structure holds all information about a managed process including
 * its name, arguments, recovery mode, and current state.
 */
typedef struct 
{
    char name [SAT_PROCESS_NAME_SIZE + 1];    /**< Process name/path */
    char args [SAT_PROCESS_ARGS_SIZE + 1];    /**< Process arguments */
    sat_process_mode_t mode;                   /**< Recovery mode */
    pid_t pid;                                 /**< Process ID (after spawn) */
    bool initialized;                          /**< Initialization status */
    sat_process_spawn_type_t type;             /**< Spawn type (with/without args) */
    
} sat_process_t;

/**
 * @brief Configuration parameters for process creation
 * 
 * This structure specifies the configuration for creating a new process
 * including its name, arguments, and recovery behavior.
 */
typedef struct 
{
    char name [SAT_PROCESS_NAME_SIZE + 1];    /**< Process name/path to execute */
    char args [SAT_PROCESS_ARGS_SIZE + 1];    /**< Process arguments (space-separated) */
    sat_process_mode_t mode;                   /**< Recovery mode */

} sat_process_args_t;

/**
 * @brief Create and configure a process object
 * 
 * Initializes a process structure with the specified configuration. The process
 * is not started until sat_process_spawn() is called. The name should be the
 * full path to the executable or a command in PATH.
 * 
 * @param[out] object Pointer to process structure to initialize
 * @param[in] args Configuration parameters including name, args, and mode
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Process successfully created and configured
 * @retval SAT_STATUS_ERROR Failed (null object/args, empty name, or invalid parameters)
 * 
 * @note Arguments are space-separated in a single string
 * @note The process name must be a valid executable path
 * @see sat_process_spawn()
 */
sat_status_t sat_process_create (sat_process_t *const object, const sat_process_args_t *const args);

/**
 * @brief Spawn the configured process
 * 
 * Forks a new child process and executes the configured program. In the parent
 * process, stores the child PID in the process structure. In the child process,
 * replaces the current process image with the new program.
 * 
 * @param[in,out] object Pointer to initialized process structure
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Process successfully spawned (parent only)
 * @retval SAT_STATUS_ERROR Failed (null object, not initialized, or fork failed)
 * 
 * @note This function uses fork() and exec() system calls
 * @note After successful spawn, object->pid contains the child process ID
 * @note The child process does not return from this function
 * @note Must call sat_process_create() before spawning
 * @see sat_process_create()
 */
sat_status_t sat_process_spawn (sat_process_t *const object);

#endif/* SAT_PROCESS_H_ */
