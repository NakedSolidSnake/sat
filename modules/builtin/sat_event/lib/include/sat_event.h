/**
 * @file sat_event.h
 * @brief Event-driven programming utilities with observer pattern
 * @author SAT Library Contributors
 * @date 2025
 * 
 * This module provides an event dispatching system implementing the observer
 * pattern. It allows components to subscribe to events and receive notifications
 * when those events are fired, enabling loose coupling between system components.
 */

#ifndef SAT_EVENT_H_
#define SAT_EVENT_H_

#include <sat_status.h>
#include <stdint.h>

/** @brief Maximum number of event-observer mappings in a dispatcher */
#define SAT_EVENT_OBSERVER_AMOUNT       10

/**
 * @brief Event handler callback function type
 * 
 * Called when an event is fired to notify registered observers.
 * 
 * @param object Pointer to the observer object
 * @param data Pointer to event-specific data passed when the event was fired
 */
typedef void (*sat_event_handler_t) (void *const object, const void *const data);

/**
 * @brief Event identifier type
 * 
 * Uniquely identifies an event type. Applications define their own event IDs.
 */
typedef uint16_t sat_event_t;

/**
 * @brief Base structure for event observers
 * 
 * Contains the essential components of an observer.
 */
typedef struct 
{
    void *object;                    /**< Pointer to the observer's object/context */
    sat_event_handler_t handler;     /**< Callback function to handle the event */
} sat_event_base_t;

/**
 * @brief Observer structure
 * 
 * Represents an entity that can observe and respond to events.
 * Applications typically embed this in their observer structures.
 */
typedef struct 
{
    sat_event_base_t base;           /**< Base observer data */
} sat_event_observer_t;

/**
 * @brief Event-to-observer mapping structure
 * 
 * Associates an event with its observer and optional context data.
 */
typedef struct 
{
    sat_event_t event;                      /**< Event identifier */
    const sat_event_observer_t *observer;   /**< Pointer to the observer */
    void *context;                          /**< Optional context data */
} sat_event_observer_map_t;

/**
 * @brief Event dispatcher structure
 * 
 * Manages event subscriptions and dispatches events to registered observers.
 */
typedef struct 
{
    sat_event_observer_map_t map[SAT_EVENT_OBSERVER_AMOUNT];  /**< Event-observer mappings */
    uint8_t amount;                                             /**< Number of registered mappings */
} sat_event_dispatcher_t;

/**
 * @brief Initializes an event dispatcher
 * 
 * Prepares the dispatcher for use by clearing all mappings and resetting state.
 * 
 * @param[out] object Pointer to the dispatcher to initialize
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Must be called before using the dispatcher
 * @see sat_event_observer_add()
 */
sat_status_t sat_event_init (sat_event_dispatcher_t *const object);

/**
 * @brief Registers an observer for an event
 * 
 * Adds a mapping between an event and an observer. When the event is fired,
 * the observer's handler will be called.
 * 
 * @param[in,out] object Pointer to the dispatcher
 * @param[in] event Event identifier to observe
 * @param[in] observer Pointer to the observer to register
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note Multiple observers can be registered for the same event
 * @note Fails if the maximum number of mappings (SAT_EVENT_OBSERVER_AMOUNT) is reached
 * @warning The observer must remain valid for the lifetime of the dispatcher
 * @see sat_event_fire()
 */
sat_status_t sat_event_observer_add (sat_event_dispatcher_t *const object, sat_event_t event, const sat_event_observer_t *const observer);

/**
 * @brief Fires an event, notifying all registered observers
 * 
 * Triggers the event by calling the handler of each observer registered
 * for that event, passing along the provided data.
 * 
 * @param[in] object Pointer to the dispatcher
 * @param[in] event Event identifier to fire
 * @param[in] data Pointer to event-specific data (can be NULL)
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note All observers registered for the event will have their handlers called
 * @note The order of notification follows registration order
 * @note If no observers are registered for the event, this is not an error
 * @see sat_event_observer_add()
 */
sat_status_t sat_event_fire (const sat_event_dispatcher_t *const object, sat_event_t event, const void *const data);


#endif/* SAT_EVENT_H_ */
