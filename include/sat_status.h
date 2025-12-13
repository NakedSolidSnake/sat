#ifndef SAT_STATUS_H
#define SAT_STATUS_H

#include <stdbool.h>

#define sat_status_break_on_error(status) \
    if (sat_status_get_result (&status) == false) \
    { \
        break; \
    }

#define sat_status_continue_on_error(status) \
    if (sat_status_get_result (&status) == false) \
    { \
        continue; \
    }


typedef struct 
{
    bool result;
    char *motive;
} sat_status_t;

sat_status_t sat_status_set (sat_status_t *object, bool result, char *motive);
bool sat_status_get_result (sat_status_t *object);
char *sat_status_get_motive (sat_status_t *object);

sat_status_t sat_status_success (sat_status_t *object);
sat_status_t sat_status_failure (sat_status_t *object, char *motive);

#endif/* SAT_STATUS_H */
