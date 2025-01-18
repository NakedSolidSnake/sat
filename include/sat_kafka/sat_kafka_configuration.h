#ifndef SAT_KAFKA_CONFIGURATION_H_
#define SAT_KAFKA_CONFIGURATION_H_

#include <librdkafka/rdkafka.h>
#include <sat_kafka_groups.h>
#include <sat_status.h>

typedef struct 
{
    rd_kafka_conf_t *configuration;
} sat_kafka_configuration_t;

sat_status_t sat_kafka_configuration_load (sat_kafka_configuration_t *object, const char *file, sat_kafka_groups_t *groups);
sat_status_t sat_kafka_configuration_unload (sat_kafka_configuration_t *object);

#endif/* SAT_KAFKA_CONFIGURATION_H_ */
