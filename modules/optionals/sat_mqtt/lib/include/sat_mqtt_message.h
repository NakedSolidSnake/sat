#ifndef SAT_MQTT_MESSAGE_H_
#define SAT_MQTT_MESSAGE_H_

#include <stdint.h>
#include <sat_mqtt_qos.h>

typedef struct
{
    char *topic;
    char *payload;
    uint32_t size;
    sat_mqtt_qos_type_t qos;
} sat_mqtt_message_t;

#endif/* SAT_MQTT_MESSAGE_H_ */

