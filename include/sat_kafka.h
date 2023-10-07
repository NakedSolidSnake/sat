#ifndef SAT_KAFKA_H_
#define SAT_KAFKA_H_

#include <librdkafka/rdkafka.h>
#include <sat_kafka_message.h>
#include <sat_kafka_base.h>
#include <sat_kafka_groups.h>
#include <sat_kafka_topics.h>
#include <sat_status.h>

#define SAT_KAFKA_ERROR_BUFFER_SIZE     512

typedef enum 
{
    sat_kafka_type_publisher,
    sat_kafka_type_subscriber
} sat_kafka_type_t;

typedef struct 
{
    sat_kafka_base_t *base;
    rd_kafka_t *instance;
    sat_kafka_type_t type;
    char error [SAT_KAFKA_ERROR_BUFFER_SIZE];
} sat_kafka_t;

typedef struct 
{
    sat_kafka_type_t type;
    const char *file;
    sat_kafka_groups_t groups;
    sat_kafka_topics_t topics;
    sat_kafka_base_t *base;
} sat_kafka_args_t;

sat_status_t sat_kafka_init (sat_kafka_t *object);
sat_status_t sat_kafka_open (sat_kafka_t *object, sat_kafka_args_t *args);
sat_status_t sat_kafka_send (sat_kafka_t *object, const char *topic, sat_kafka_message_t *message, int timeout);
sat_status_t sat_kafka_poll (sat_kafka_t *object, int timeout);
sat_status_t sat_kafka_close (sat_kafka_t *object);

#endif/* SAT_KAFKA_H_ */
