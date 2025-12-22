#ifndef SAT_KAFKA_MESSAGE_H_
#define SAT_KAFKA_MESSAGE_H_

typedef struct 
{
    void *data;
    size_t size;
} sat_kafka_buffer_t;

typedef struct
{
    const char *topic;
    sat_kafka_buffer_t key;
    sat_kafka_buffer_t payload;
} sat_kafka_message_t;

#endif/* SAT_KAFKA_MESSAGE_H_ */
