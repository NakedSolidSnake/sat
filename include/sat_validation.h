#ifndef SAT_VALIDATION_H
#define SAT_VALIDATION_H

#include <stdint.h>
#include <sat_status.h>

#define SAT_VALIDATION_CRITERIA_AMOUNT      10

typedef sat_status_t (*sat_criteria_t) (void *data);

typedef struct 
{
    sat_criteria_t criterias [SAT_VALIDATION_CRITERIA_AMOUNT];
    uint8_t amount;
} sat_validation_t;

sat_status_t sat_validation_init (sat_validation_t *object);
sat_status_t sat_validation_add (sat_validation_t *object, sat_criteria_t criteria);
sat_status_t sat_validation_verify (sat_validation_t *object, void *data);

#endif/* SAT_VALIDATION_H */
