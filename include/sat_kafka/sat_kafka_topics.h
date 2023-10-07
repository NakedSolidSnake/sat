#ifndef SAT_KAFKA_TOPICS_H_
#define SAT_KAFKA_TOPICS_H_

#include <stdint.h>

typedef struct 
{
    char **list;
    uint8_t amount;
} sat_kafka_topics_t;

#endif/* SAT_KAFKA_TOPICS_H_ */
