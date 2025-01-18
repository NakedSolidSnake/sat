#ifndef SAT_ITERATOR_H_
#define SAT_ITERATOR_H_

#include <sat_status.h>
#include <stdint.h>

typedef struct 
{
    void *object;
    void *(*next) (void *object, uint32_t index);
    uint32_t (*get_amount) (void *object);

} sat_iterator_base_t;

typedef struct 
{
    uint32_t index;
    sat_iterator_base_t *base;
    uint32_t amount;
    bool initialized;

} sat_iterator_t;

sat_status_t sat_iterator_open (sat_iterator_t *object, sat_iterator_base_t *base);
void *sat_iterator_next (sat_iterator_t *object);

#endif/* SAT_ITERATOR_H_ */
