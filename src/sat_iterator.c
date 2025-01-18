#include <sat_iterator.h>
#include <string.h>

static sat_status_t sat_iterator_is_base_valid (sat_iterator_base_t *base);
static void sat_iterator_get_info (sat_iterator_t *object, sat_iterator_base_t *base);

sat_status_t sat_iterator_open (sat_iterator_t *object, sat_iterator_base_t *base)
{
    sat_status_t status = sat_status_set (&status, false, "sat iterator open error");

    if (object != NULL)
    {

        do 
        {
            status = sat_iterator_is_base_valid (base);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_iterator_get_info (object, base);

            sat_status_set (&status, true, false);

        } while (false);
    }

    return status;
}

void *sat_iterator_next (sat_iterator_t *object)
{
    void *item = NULL;

    if (object != NULL && object->initialized == true && object->index < object->amount)
    {
        item = object->base->next (object->base, object->index);

        object->index ++;
    }

    return item;
}

static sat_status_t sat_iterator_is_base_valid (sat_iterator_base_t *base)
{
    sat_status_t status = sat_status_set (&status, false, "sat iterator is base valid error");

    if (base != NULL &&
        base->object != NULL &&
        base->get_amount != NULL &&
        base->next != NULL)
    {
        sat_status_set (&status, true, false);
    }

    return status;
}

static void sat_iterator_get_info (sat_iterator_t *object, sat_iterator_base_t *base)
{
    object->base = base;
    object->index = 0;
    object->initialized = true;
    object->amount = base->get_amount (base);
}