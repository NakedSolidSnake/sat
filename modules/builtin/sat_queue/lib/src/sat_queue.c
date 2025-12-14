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
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat queue create error: null object");
            break;
        }

        if (object_size == 0)
        {
            sat_status_failure (&status, "sat queue create error: zero object size");
            break;
        }

        *object = (sat_queue_t *) calloc (1, sizeof (sat_queue_t));
        if (*object == NULL)
        {
            sat_status_failure (&status, "sat queue create error: allocation failed");
            break;
        }

        (*object)->object_size = object_size;
        (*object)->start = NULL;
        (*object)->end = NULL;

    } while (false);

    return status;
}

sat_status_t sat_queue_enqueue (sat_queue_t *const object, const void *const data)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat queue enqueue error: null object");
            break;
        }

        if (data == NULL)
        {
            sat_status_failure (&status, "sat queue enqueue error: null data");
            break;
        }

        sat_linked_list_internal_t *element = calloc (1, sizeof (sat_linked_list_internal_t));
        if (element == NULL)
        {
            sat_status_failure (&status, "sat queue enqueue error: element allocation failed");
            break;
        }

        element->data = calloc (1, object->object_size);
        if (element->data == NULL)
        {
            free (element);
            sat_status_failure (&status, "sat queue enqueue error: data allocation failed");
            break;
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

    } while (false);

    return status;
}

sat_status_t sat_queue_dequeue (sat_queue_t *const object, void *const data)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat queue dequeue error: null object");
            break;
        }

        if (data == NULL)
        {
            sat_status_failure (&status, "sat queue dequeue error: null data");
            break;
        }

        if (object->amount == 0)
        {
            sat_status_set (&status, false, "sat queue dequeue error: queue is empty");
            break;
        }

        sat_linked_list_internal_t *element = object->start;

        memcpy (data, element->data, object->object_size);

        object->start = element->next;

        if (object->start == NULL)
            object->end = NULL;

        object->amount --;

        free (element->data);
        free (element);

    } while (false);

    return status;
}

sat_status_t sat_queue_get_size (const sat_queue_t *const object, uint32_t *const size)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat queue get size error: null object");
            break;
        }

        if (size == NULL)
        {
            sat_status_failure (&status, "sat queue get size error: null size pointer");
            break;
        }

        *size = object->amount;

    } while (false);

    return status;
}

sat_status_t sat_queue_debug (const sat_queue_t *const object, sat_queue_print_t print)
{   
    sat_status_t status = sat_status_success (&status);

     do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat queue debug error: null object");
            break;
        }

        if (print == NULL)
        {
            sat_status_failure (&status, "sat queue debug error: null print function");
            break;
        }

        sat_linked_list_internal_t *element = object->start;

        while (element != NULL)
        {
            print (element->data);
            element = element->next;
        }

    } while (false);

    return status;
}

sat_status_t sat_queue_destroy (sat_queue_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_failure (&status, "sat queue destroy error: null object");
            break;
        }

        sat_linked_list_internal_t *element = object->start;

        while (element != NULL)
        {
            sat_linked_list_internal_t *temp = element->next;
            free (element->data);
            free (element);

            element = temp;            
        }

        free (object);

    } while (false);

    return status;
}