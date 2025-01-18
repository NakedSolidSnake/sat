#ifndef SAT_MQTT_QOS_H_
#define SAT_MQTT_QOS_H_

typedef enum 
{
    sat_mqtt_qos_type_fire_and_forget,
    sat_mqtt_qos_type_at_least_one,
    sat_mqtt_qos_type_once_and_only_once
} sat_mqtt_qos_type_t;

#endif/* SAT_MQTT_QOS_H_ */
