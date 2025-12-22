#ifndef SAT_KAFKA_BASE_H_
#define SAT_KAFKA_BASE_H_

#include <stdbool.h>
#include <sat_kafka_message.h>

typedef struct 
{
    void *object;
    bool (*on_record) (void *object, sat_kafka_message_t *message);
} sat_kafka_base_t;

#endif/* SAT_KAFKA_BASE_H_ */
