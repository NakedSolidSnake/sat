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

sat_status_t sat_queue_create (sat_queue_t **object, uint32_t object_size)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue create error");

    if (object != NULL && object_size > 0)
    {
        sat_status_set (&status, false, "sat queue create context error");

        *object = (sat_queue_t *) calloc (1, sizeof (sat_queue_t));

        if (*object != NULL)
        {
            (*object)->object_size = object_size;
            (*object)->start = NULL;
            (*object)->end = NULL;

            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_queue_enqueue (sat_queue_t *object, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue enqueue error");

    if (object != NULL && data != NULL)
    {
        sat_status_set (&status, false, "sat queue item allocation error");

        sat_linked_list_internal_t *element = calloc (1, sizeof (sat_linked_list_internal_t));

        if (element != NULL)
        {
            element->data = calloc (1, object->object_size);
            if (element->data != NULL)
            {
                memcpy (element->data, data, object->object_size);

                if (object->end != NULL)
                    object->end->next = element;
                else 
                    object->start = element;

                object->end = element;
                
                object->amount ++;

                sat_status_set (&status, true, "");
            }

            else
            {
                free (element);
                sat_status_set (&status, false, "sat queue data allocation error");
            }
        }
    }

    return status;
}

sat_status_t sat_queue_dequeue (sat_queue_t *object, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue dequeue error");

    if (object != NULL && object->amount > 0 && data != NULL)
    {
        sat_status_set (&status, false, "sat queue dequeue not found error");

        sat_linked_list_internal_t *element = object->start;

        memcpy (data, element->data, object->object_size);

        object->start = element->next;

        if (object->start == NULL)
            object->end = NULL;

        object->amount --;

        free (element->data);
        free (element);
        
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_queue_get_size (sat_queue_t *object, uint32_t *size)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue get size error");

    if (object != NULL && size != NULL)
    {
        *size = object->amount;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_queue_debug (sat_queue_t *object, sat_queue_print_t print)
{   
    sat_status_t status = sat_status_set (&status, false, "sat queue debug error");

    if (object != NULL && print != NULL)
    {
        sat_linked_list_internal_t *element = object->start;

        while (element != NULL)
        {
            print (element->data);
            element = element->next;
        }
        
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_queue_destroy (sat_queue_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue destroy error");

    if (object != NULL)
    {
      
        sat_linked_list_internal_t *element = object->start;

        while (element != NULL)
        {
            sat_linked_list_internal_t *temp = element->next;
            free (element->data);
            free (element);

            element = temp;            
        }

        free (object);
        
        sat_status_set (&status, true, "");
    }

    return status;
}