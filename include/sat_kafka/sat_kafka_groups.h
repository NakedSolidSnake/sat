#ifndef SAT_KAFKA_GROUPS_H_
#define SAT_KAFKA_GROUPS_H_

#include <stdint.h>

typedef struct 
{
    char **list;
    uint8_t amount;
} sat_kafka_groups_t;

#endif/* SAT_KAFKA_GROUPS_H_ */
