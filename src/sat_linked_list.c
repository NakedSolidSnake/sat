#include <sat_linked_list.h>
#include <string.h>
#include <stdlib.h>

typedef struct sat_linked_list_internal_t sat_linked_list_internal_t;

struct sat_linked_list_internal_t
{
    void *data;
    sat_linked_list_internal_t *next;
};

struct sat_linked_list_t
{
    uint32_t object_size;
    uint32_t amount;
    sat_linked_list_internal_t *list;
};

sat_status_t sat_linked_list_create (sat_linked_list_t **object, uint32_t object_size)
{
    sat_status_t status = sat_status_set (&status, false, "sat liked list create error");

    if (object != NULL && object_size > 0)
    {
        sat_status_set (&status, false, "sat liked list buffer error");

        *object = (sat_linked_list_t *) calloc (1, sizeof (sat_linked_list_t));

        if (*object != NULL)
        {
            (*object)->object_size = object_size;
            (*object)->list = NULL;

            sat_status_set (&status, true, "");
        }

    }

    return status;
}

sat_status_t sat_linked_list_insert (sat_linked_list_t *object, const void *element)
{
    sat_status_t status = sat_status_set (&status, false, "sat liked list insert error");

    if (object != NULL && element != NULL)
    {
        sat_status_set (&status, false, "sat liked list element allocation error");

        sat_linked_list_internal_t *_element = calloc (1, sizeof (sat_linked_list_internal_t));

        if (_element != NULL)
        {
            _element->data = calloc (1, object->object_size);
            memcpy (_element->data, element, object->object_size);

            _element->next = object->list;
            object->list = _element;
            object->amount++;

            sat_status_set (&status, true, "");
        }
        
    }

    return status;
}

sat_status_t sat_linked_list_remove (sat_linked_list_t *object, sat_linked_list_compare_t compare, const void *param)
{
    sat_status_t status = sat_status_set (&status, false, "sat liked list remove error");

    if (object != NULL && compare != NULL && param != NULL)
    {
        sat_status_set (&status, false, "sat liked list remove not found error");

        sat_linked_list_internal_t *element = object->list;
        sat_linked_list_internal_t *temp;

        while (element != NULL)
        {

            if (compare (element->data, (void *)param) == true)
            {

                if (element == object->list)
                    object->list = element->next;
                else
                    temp->next = element->next;

                free (element->data);
                free (element);

                object->amount --;

                sat_status_set (&status, true, "");

                break;
            }

            temp = element;
            element = element->next;
        }
    }

    return status;
}

sat_status_t sat_linked_list_get (sat_linked_list_t *object, sat_linked_list_compare_t compare, const void *param, void *element)
{
    sat_status_t status = sat_status_set (&status, false, "sat liked list get error");

    if (object != NULL && compare != NULL && param != NULL && element != NULL)
    {
        sat_status_set (&status, false, "sat liked list get not found error");

        sat_linked_list_internal_t *_element = object->list;

        while (_element != NULL)
        {

            if (compare (_element->data, (void *)param) == true)
            {
                memcpy (element, _element->data, object->object_size);
                sat_status_set (&status, true, "");
                break;
            }

            _element = _element->next;
        }
        
    }

    return status;
}

sat_status_t sat_linked_list_get_size (sat_linked_list_t *object, uint32_t *size)
{
    sat_status_t status = sat_status_set (&status, false, "sat liked list get size error");

    if (object != NULL && size != NULL)
    {
        *size = object->amount;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_linked_list_debug (sat_linked_list_t *object, sat_linked_list_print_t print)
{
    sat_status_t status = sat_status_set (&status, false, "sat liked list debug error");

    if (object != NULL && print)
    {   

        sat_linked_list_internal_t *element = object->list;

        while (element != NULL)
        {
            print (element->data);
            element = element->next;
        }
        
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_linked_list_destroy (sat_linked_list_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat liked list destroy error");

    if (object != NULL)
    {
        sat_linked_list_internal_t *element = object->list;

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