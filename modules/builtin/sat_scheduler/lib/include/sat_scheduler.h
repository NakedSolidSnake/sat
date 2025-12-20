/**
 * @file sat_scheduler.h
 * @brief Event scheduler with periodic and one-shot timers
 * 
 * This module provides a flexible event scheduling system that runs events
 * in a separate thread based on configurable timeouts. It supports both
 * periodic events (repeating) and one-shot events (single execution).
 * 
 * The scheduler uses millisecond precision timing and can manage multiple
 * events with different intervals. Events are identified by name and can
 * carry custom context data.
 */

#ifndef SAT_SCHEDULER_H_
#define SAT_SCHEDULER_H_

#include <sat_status.h>
#include <sat_set.h>
#include <stdint.h>
#include <pthread.h>

/**
 * @brief Event handler function type
 * 
 * User-defined callback function that is executed when an event's
 * timeout expires. The function receives a context object pointer
 * that was associated with the event.
 * 
 * @param object Context pointer passed to the event handler
 */
typedef void (*sat_scheduler_handler_t) (void *object);

/**
 * @brief Scheduler instance structure
 * 
 * Represents a scheduler that manages a collection of timed events.
 * The scheduler runs in a separate thread and triggers event handlers
 * when their timeouts expire.
 */
typedef struct 
{
    sat_set_t *set;              /**< Set containing scheduled events */
    uint16_t events_amount;      /**< Maximum number of events */
    pthread_t thread;            /**< Scheduler thread handle */
    bool running;                /**< Scheduler running state */
} sat_scheduler_t;

/**
 * @brief Scheduler mode enumeration
 * 
 * Determines how the scheduler manages its event collection.
 */
typedef enum
{
    sat_scheduler_mode_static,   /**< Fixed-size event collection */
    sat_scheduler_mode_dynamic,  /**< Dynamic event collection */
} sat_scheduler_mode_t;

/**
 * @brief Event type enumeration
 * 
 * Defines whether an event executes once or repeats periodically.
 */
typedef enum
{
    sat_scheduler_type_periodic, /**< Event repeats after each timeout */
    sat_scheduler_type_one_shot, /**< Event executes once then is removed */
} sat_scheduler_type_t;

/**
 * @brief Scheduler configuration structure
 * 
 * Configuration parameters for creating a scheduler instance.
 */
typedef struct
{
    uint16_t event_amount;           /**< Maximum number of events */
    sat_scheduler_mode_t mode;       /**< Scheduler mode (static/dynamic) */
} sat_scheduler_args_t;

/**
 * @brief Scheduled event structure
 * 
 * Represents a single scheduled event with its handler, timing, and context.
 */
typedef struct
{
    void *object;                    /**< Context object for handler */
    char *name;                      /**< Event name (must be unique) */
    sat_scheduler_handler_t handler; /**< Event handler function */
    sat_scheduler_type_t type;       /**< Event type (periodic/one-shot) */
    uint64_t timeout;                /**< Timeout in milliseconds */
    uint64_t last_update;            /**< Last execution timestamp */
} sat_scheduler_event_t;

/**
 * @brief Initialize a scheduler object
 * 
 * Initializes a scheduler instance by clearing all fields.
 * Must be called before sat_scheduler_open().
 * 
 * @param object Pointer to scheduler object to initialize
 * @return Status indicating success or failure
 */
sat_status_t sat_scheduler_init (sat_scheduler_t *const object);

/**
 * @brief Open and configure a scheduler
 * 
 * Configures a scheduler with the specified parameters. Creates the
 * internal event collection with the specified capacity and mode.
 * 
 * @param object Pointer to initialized scheduler object
 * @param args Pointer to configuration structure
 * @return Status indicating success or failure
 */
sat_status_t sat_scheduler_open (sat_scheduler_t *const object, const sat_scheduler_args_t *const args);

/**
 * @brief Add an event to the scheduler
 * 
 * Adds a new event to the scheduler's event collection. The event
 * will not execute until the scheduler is started. Event names must
 * be unique within the scheduler.
 * 
 * @param object Pointer to scheduler object
 * @param event Pointer to event structure to add
 * @return Status indicating success or failure
 */
sat_status_t sat_scheduler_add_event (sat_scheduler_t *const object, const sat_scheduler_event_t *const event);

/**
 * @brief Start the scheduler
 * 
 * Starts the scheduler thread, which begins monitoring events and
 * executing handlers when timeouts expire. All events begin timing
 * from the moment the scheduler starts.
 * 
 * @param object Pointer to scheduler object
 * @return Status indicating success or failure
 */
sat_status_t sat_scheduler_start (sat_scheduler_t *const object);

/**
 * @brief Stop the scheduler
 * 
 * Stops the scheduler thread and waits for it to terminate. Events
 * are preserved but no longer execute until the scheduler is restarted.
 * 
 * @param object Pointer to scheduler object
 * @return Status indicating success or failure
 */
sat_status_t sat_scheduler_stop (sat_scheduler_t *const object);

/**
 * @brief Check if scheduler is running
 * 
 * Checks whether the scheduler thread is currently active.
 * 
 * @param object Pointer to scheduler object
 * @return Status indicating success if running, failure if not running
 */
sat_status_t sat_scheduler_is_running (sat_scheduler_t *const object);

/**
 * @brief Get current number of events
 * 
 * Retrieves the current number of events in the scheduler.
 * For one-shot events, this count may decrease as events execute.
 * 
 * @param object Pointer to scheduler object
 * @param amount Pointer to store the event count
 * @return Status indicating success or failure
 */
sat_status_t sat_scheduler_get_amount (const sat_scheduler_t *const object, uint16_t *const amount);

/**
 * @brief Close and cleanup the scheduler
 * 
 * Stops the scheduler if running, destroys the event collection,
 * and releases all resources. The scheduler object is cleared and
 * must be reinitialized before reuse.
 * 
 * @param object Pointer to scheduler object
 * @return Status indicating success or failure
 */
sat_status_t sat_scheduler_close (sat_scheduler_t *const object);


#endif/* SAT_SCHEDULER_H_ */
