#ifndef SAT_ITERATOR_H_
#define SAT_ITERATOR_H_

#include <sat_status.h>
#include <stdint.h>

typedef enum
{
    sat_iterator_type_index,
    sat_iterator_type_address,
    sat_iterator_type_unknown
} sat_iterator_type_t;

typedef struct 
{
    void *object;
    void *(*next) (const void *const object, const uint32_t index);
    uint32_t (*get_amount) (const void *const object);
    void *(*get_address) (const void *const object);
    void *(*get_next_address) (const void *const object, const void *const address);
    void *(*get_data) (const void *const address);

} sat_iterator_base_t;

typedef struct 
{
    uint32_t index;
    const sat_iterator_base_t *base;
    uint32_t amount;
    bool initialized;

    sat_iterator_type_t type;
    struct
    {
        void *next;
        void *current;
    } address;

} sat_iterator_t;

sat_status_t sat_iterator_open (sat_iterator_t *const object, const sat_iterator_base_t *const base);
void *sat_iterator_next (sat_iterator_t *const object);

#endif/* SAT_ITERATOR_H_ */
