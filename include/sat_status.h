#ifndef SAT_STATUS_H
#define SAT_STATUS_H

#include <stdbool.h>

typedef struct 
{
    bool result;
    char *motive;
} sat_status_t;

sat_status_t sat_status_set (sat_status_t *object, bool result, char *motive);
bool sat_status_get_result (sat_status_t *object);
char *sat_status_get_motive (sat_status_t *object);

#endif/* SAT_STATUS_H */
