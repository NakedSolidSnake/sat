#ifndef SAT_PROPERTIES_H_
#define SAT_PROPERTIES_H_

#include <sat_status.h>
#include <stdint.h>

#define SAT_PROPERTIES_ITEMS_AMOUNT     250

typedef enum 
{
    sat_properties_type_integer,
    sat_properties_type_string,
    sat_properties_type_decimal
} sat_properties_type_t;

typedef struct 
{
    char *filename;
} sat_properties_t;

typedef struct
{
    char *group;
    char *field;
    sat_properties_type_t type;
    uint16_t size;
    void *data;
} sat_properties_item_t;

typedef struct
{
    sat_properties_item_t items [SAT_PROPERTIES_ITEMS_AMOUNT];
    uint8_t amount;
} sat_properties_map_t;

sat_status_t sat_properties_open (sat_properties_t *object, char *filename);
sat_status_t sat_properties_read (sat_properties_t *object, sat_properties_map_t *map);

#endif/* SAT_PROPERTIES_H_ */
