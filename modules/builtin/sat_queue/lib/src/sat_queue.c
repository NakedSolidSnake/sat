#include <sat_queue.h>
#include <string.h>
#include <stdlib.h>

typedef struct sat_linked_list_internal_t sat_linked_list_internal_t;

struct sat_linked_list_internal_t
{
    void *data;
    sat_linked_list_internal_t *next;
};

struct sat_queue_t
{
    uint32_t object_size;
    uint32_t amount;
    sat_linked_list_internal_t *start;
    sat_linked_list_internal_t *end;
};

sat_status_t sat_queue_create (sat_queue_t **const object, uint32_t object_size)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_equals (object_size, 0, "zero object size");
    
    *object = (sat_queue_t *) calloc (1, sizeof (sat_queue_t));
    sat_status_return_on_null (*object, "allocation failed");

    (*object)->object_size = object_size;
    (*object)->start = NULL;
    (*object)->end = NULL;

    sat_status_return_on_success ();
}

sat_status_t sat_queue_enqueue (sat_queue_t *const object, const void *const data)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (data, "null data pointer");
    sat_linked_list_internal_t *element = calloc (1, sizeof (sat_linked_list_internal_t));
    sat_status_return_on_null (element, "element allocation failed");

    element->data = calloc (1, object->object_size);
    if (element->data == NULL)
    {
        free (element);
        sat_status_return_on_failure ("data allocation failed");
    }

    memcpy (element->data, data, object->object_size);

    if (object->end != NULL)
    {
        object->end->next = element;
    }
    else
    {
        object->start = element;
    }

    object->end = element;
    
    object->amount ++;

    sat_status_return_on_success ();
}

sat_status_t sat_queue_dequeue (sat_queue_t *const object, void *const data)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (data, "null data pointer");
    sat_status_return_on_equals (object->amount, 0, "queue is empty");

    sat_linked_list_internal_t *element = object->start;

    memcpy (data, element->data, object->object_size);

    object->start = element->next;

    if (object->start == NULL)
        object->end = NULL;

    object->amount --;

    free (element->data);
    free (element);

    sat_status_return_on_success ();
}

sat_status_t sat_queue_get_size (const sat_queue_t *const object, uint32_t *const size)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (size, "null size pointer");

    *size = object->amount;

    sat_status_return_on_success ();
}

sat_status_t sat_queue_debug (const sat_queue_t *const object, sat_queue_print_t print)
{   
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_null (print, "null print function");

    sat_linked_list_internal_t *element = object->start;

    while (element != NULL)
    {
        print (element->data);
        element = element->next;
    }

    sat_status_return_on_success ();
}

sat_status_t sat_queue_destroy (sat_queue_t *const object)
{
    sat_status_return_on_null (object, "null object");

    sat_linked_list_internal_t *element = object->start;

    while (element != NULL)
    {
        sat_linked_list_internal_t *temp = element->next;
        free (element->data);
        free (element);

        element = temp;
    }

    free (object);

    sat_status_return_on_success ();
}