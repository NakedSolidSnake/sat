/**
 * @file sat_process_monitor.h
 * @brief Process monitoring and automatic recovery system
 * @author SAT Library Team
 * @date December 2025
 * 
 * This module provides a process monitoring system that tracks multiple child
 * processes and automatically restarts them based on their configured recovery
 * mode. It's designed for building reliable systems that require process
 * supervision and automatic failure recovery.
 */

#ifndef SAT_PROCESS_MONITOR_H
#define SAT_PROCESS_MONITOR_H

#include <sat_array.h>
#include <sat_process.h>

/**
 * @brief Process monitor structure
 * 
 * This structure manages a collection of processes, monitoring their status
 * and handling automatic recovery when processes terminate unexpectedly.
 */
typedef struct 
{
    sat_array_t *array;  /**< Array of managed processes */
    bool initialized;    /**< Initialization status */
    bool start;          /**< Monitor running status */

} sat_process_monitor_t;

/**
 * @brief Initialize the process monitor
 * 
 * Creates and initializes a process monitor instance with an empty process list.
 * The monitor is ready to accept process registrations but not yet started.
 * 
 * @param[out] object Pointer to process monitor structure to initialize
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Monitor successfully initialized
 * @retval SAT_STATUS_ERROR Failed (null object or array creation failed)
 * 
 * @see sat_process_monitor_add(), sat_process_monitor_start()
 */
sat_status_t sat_process_monitor_open (sat_process_monitor_t *const object);

/**
 * @brief Add a process to the monitor
 * 
 * Registers a process with the monitor for tracking and automatic recovery.
 * The process should be created with sat_process_create() before adding.
 * Processes can be added before or after calling sat_process_monitor_start().
 * 
 * @param[in,out] object Pointer to initialized process monitor
 * @param[in] process Process structure to add (copied into monitor)
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Process successfully added
 * @retval SAT_STATUS_ERROR Failed (null object, not initialized, or array add failed)
 * 
 * @note The process is copied, so the original can be reused
 * @note Processes are automatically spawned when added to a running monitor
 * @see sat_process_create(), sat_process_monitor_start()
 */
sat_status_t sat_process_monitor_add (sat_process_monitor_t *const object, sat_process_t process);

/**
 * @brief Start the process monitor
 * 
 * Activates the monitor, enabling process scanning and automatic recovery.
 * All processes registered with sat_process_monitor_add() are spawned when
 * the monitor starts.
 * 
 * @param[in,out] object Pointer to initialized process monitor
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Monitor successfully started
 * @retval SAT_STATUS_ERROR Failed (null object or not initialized)
 * 
 * @note After starting, call sat_process_monitor_scan() periodically
 * @see sat_process_monitor_scan(), sat_process_monitor_stop()
 */
sat_status_t sat_process_monitor_start (sat_process_monitor_t *const object);

/**
 * @brief Scan for terminated processes and handle recovery
 * 
 * Checks all monitored processes for termination and handles recovery according
 * to each process's configured mode. This function should be called periodically
 * in a loop to maintain process supervision.
 * 
 * @param[in,out] object Pointer to started process monitor
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Scan completed successfully
 * @retval SAT_STATUS_ERROR Failed (null object, not initialized, or not started)
 * 
 * @note Uses waitpid() with WNOHANG for non-blocking operation
 * @note Processes with mode sat_process_mode_recover are restarted automatically
 * @note Critical processes cause monitor termination if restart fails
 * @see sat_process_monitor_start()
 */
sat_status_t sat_process_monitor_scan (sat_process_monitor_t *const object);

/**
 * @brief Stop the process monitor and terminate all processes
 * 
 * Stops monitoring, terminates all managed processes gracefully, and cleans
 * up resources. Sends SIGTERM to all processes and waits for them to exit.
 * 
 * @param[in,out] object Pointer to process monitor to stop
 * @return Status indicating success or failure
 * @retval SAT_STATUS_OK Monitor successfully stopped
 * @retval SAT_STATUS_ERROR Failed (null object or not initialized)
 * 
 * @note All processes are terminated regardless of their recovery mode
 * @note Blocks until all processes have terminated
 * @see sat_process_monitor_start()
 */
sat_status_t sat_process_monitor_stop (sat_process_monitor_t *const object);
#endif /* SAT_PROCESS_MONITOR_H */
