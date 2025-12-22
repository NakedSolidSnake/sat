#ifndef SAT_GTK3_BOX_H_
#define SAT_GTK3_BOX_H_


#include <sat_status.h>
#include <stdint.h>
#include <sat_gtk3_widget.h>

typedef enum 
{
    sat_gtk3_box_type_horizontal,
    sat_gtk3_box_type_vertical,
} sat_gtk3_box_type_t;

typedef enum 
{
    sat_gtk3_box_add_start,
    sat_gtk3_box_add_end
} sat_gtk3_box_add_t;

typedef struct 
{
    sat_gtk3_box_add_t add_type;
    bool expand;
    bool fill;
    uint32_t padding;

} sat_gtk3_box_add_properties_t;

typedef struct 
{
    sat_gtk3_widget_t box;
    bool initialized;
} sat_gtk3_box_t;

typedef struct 
{
    sat_gtk3_box_type_t type;
    int space;
} sat_gtk3_box_args_t;

sat_status_t sat_gtk3_box_create (sat_gtk3_box_t *object, sat_gtk3_box_args_t *args);
sat_status_t sat_gtk3_box_add_widget (sat_gtk3_box_t *object, sat_gtk3_widget_t *widget, sat_gtk3_box_add_properties_t properties);

#endif/* SAT_GTK3_BOX_H_ */
