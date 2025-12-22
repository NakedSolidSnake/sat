#ifndef SAT_MQTT_H_
#define SAT_MQTT_H_

#include <sat_status.h>
#include <sat_mqtt_persistence_type.h>
#include <sat_mqtt_message.h>

typedef void (*sat_mqtt_on_receive_t) (void *object, sat_mqtt_message_t message);
typedef void (*sat_mqtt_on_lost_connection_t) (void *object, char *motive);

typedef enum 
{
    sat_mqtt_type_subscriber,
    sat_mqtt_type_publisher
} sat_mqtt_type_t;

typedef struct
{
    void *handle;
    sat_mqtt_type_t type;
    void *data;

    struct 
    {
        sat_mqtt_on_receive_t on_receive;
        sat_mqtt_on_lost_connection_t on_lost_connection;
    } events;
} sat_mqtt_t;


typedef struct
{
    const char *address;
    const char *id;
    sat_mqtt_persistence_type_t persistence;
    sat_mqtt_type_t type;
    void *data;

    struct 
    {
        sat_mqtt_on_receive_t on_receive;
        sat_mqtt_on_lost_connection_t on_lost_connection;
    } events;
} sat_mqtt_args_t;

sat_status_t sat_mqtt_init (sat_mqtt_t *object);
sat_status_t sat_mqtt_open (sat_mqtt_t *object, sat_mqtt_args_t *args);
sat_status_t sat_mqtt_publish (sat_mqtt_t *object, sat_mqtt_message_t *message);
sat_status_t sat_mqtt_subscribe (sat_mqtt_t *object, char *topic, sat_mqtt_qos_type_t qos);
sat_status_t sat_mqtt_close (sat_mqtt_t *object);

#endif/* SAT_MQTT_H_ */
